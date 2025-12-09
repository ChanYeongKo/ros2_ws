#include "linedetect_wsl/sub.hpp"
using namespace cv;
using namespace std;
using std::placeholders::_1;

LineSubscriber::LineSubscriber(): Node("camsub_wsl"), prev_x_(-1.0)
{
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

    sub_ = this->create_subscription<sensor_msgs::msg::CompressedImage>(
        "image/compressed",
        qos_profile,
        std::bind(&LineSubscriber::callback, this, std::placeholders::_1)
    );

    RCLCPP_INFO(this->get_logger(), "LineSubscriber Node Started");
}

void LineSubscriber::callback(
        const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    auto startTime = chrono::steady_clock::now();

    Mat frame, gray, binary, labels, stats, centroids, corrected_gray;
    double current_mean, target_mean, diff, min_dist, center_x, target_x, error = 0.0;
    int h, w, cnt, best_idx;
    const double MAX_MOVE_THRESHOLD = 80.0;

    frame = cv::imdecode(Mat(msg->data), IMREAD_COLOR);
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    Scalar avg = mean(gray);
    current_mean = avg.val[0];
    target_mean = 160.0;
    diff = target_mean - current_mean;
    corrected_gray = gray + diff;

    threshold(corrected_gray, binary, 210, 255, THRESH_BINARY);

    h = binary.rows;
    w = binary.cols;
    Rect roi(0, 3 * h / 4, w, h / 4);

    Mat binary_half = binary(roi).clone();

    cnt = connectedComponentsWithStats(binary_half, labels, stats, centroids);

    center_x = binary_half.cols / 2.0;
    target_x = (prev_x_ != -1.0) ? prev_x_ : center_x;

    best_idx = -1;
    min_dist = DBL_MAX;

    for (int i = 1; i < cnt; i++)
    {
        int* p = stats.ptr<int>(i);
        if (p[4] < 200 || p[4] > 7000) continue;

        double cx = centroids.at<double>(i, 0);
        double dist = abs(target_x - cx);

        if (dist < min_dist)
        {
            min_dist = dist;
            best_idx = i;
        }
    }

    cvtColor(binary_half, binary_half, COLOR_GRAY2BGR);

    if (best_idx != -1)
    {
        int* q = stats.ptr<int>(best_idx);

        if (min_dist < MAX_MOVE_THRESHOLD)
        {
            prev_x_ = centroids.at<double>(best_idx, 0);
            rectangle(binary_half, Rect(q[0], q[1], q[2], q[3]), Scalar(0,0,255), 2);
            circle(binary_half, Point(prev_x_, centroids.at<double>(best_idx,1)),
                   2, Scalar(0,0,255), -1);
        }
        else
        {
            rectangle(binary_half, Rect(q[0], q[1], q[2], q[3]), Scalar(255,0,0), 2);
            circle(binary_half, Point(centroids.at<double>(best_idx,0),
                                      centroids.at<double>(best_idx,1)),
                   2, Scalar(255,0,0), -1);
        }

        error = center_x - prev_x_;
    }
    else
    {
        error = 0.0;
    }

    imshow("Original", frame);
    imshow("Binary ROI Result", binary_half);
    waitKey(1);

    auto endTime = chrono::steady_clock::now();
    float totalTime = chrono::duration<double, milli>(endTime - startTime).count();

    cout << fixed << setprecision(0)
         << "Error: " << error << ", time: " << totalTime << " msec" << endl;
}