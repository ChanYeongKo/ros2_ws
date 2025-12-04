#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
using namespace std;
using namespace cv;

int main()
{
	// 1. 비디오 캡처 초기화 (0: 웹캠, "video.mp4": 동영상 파일)
	VideoCapture cap("C:\\Users\\kochan\\Desktop\\자율주행\\7_lt_ccw_100rpm_in.mp4");

	if (!cap.isOpened()) {
		cerr << "에러: 카메라를 열 수 없습니다." << endl;
		return -1;
	}

	Mat frame, gray, binary, labels, stats, centroids, corrected_gray;
	double current_mean, target_mean, diff, min_dist, center_x, target_x, error = 0.0;
	int h, w, cnt, best_idx;
	static double prev_x = -1.0;//static으로 선언해서 while문이 다시 실행되어도 이전 값이 유지된다.
	const double MAX_MOVE_THRESHOLD = 80.0; //const로 MAX_MOVE_THRESHOLD의 값을 고정한다.
	while (true)
	{
		auto startTime = std::chrono::steady_clock::now(); // 시작시간 저장
		// 2. 프레임 읽기
		cap >> frame;
		if (frame.empty()) break; // 영상이 끝나면 종료

		// === 여기서부터 영상처리 로직 (ROS의 콜백 함수 내부 내용) ===

		// 그레이스케일 변환
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		// 밝기 보정 로직
		Scalar avg = mean(gray);
		current_mean = avg.val[0];
		target_mean = 160.0;
		diff = target_mean - current_mean;

		// 밝기 보정 (포화 연산 자동 적용)
		corrected_gray = gray + diff;

		// 이진화
		threshold(corrected_gray, binary, 210, 255, THRESH_BINARY);

		// ROI 설정 (하단 1/4)
		h = binary.rows;
		w = binary.cols;
		Rect roi(0, 3 * h / 4, w, h / 4);
		Mat binary_half = binary(roi); // ROI 이미지 참조

		// 객체 검출 (레이블링)
		cnt = connectedComponentsWithStats(binary_half, labels, stats, centroids);


		cvtColor(binary_half, binary_half, COLOR_GRAY2BGR);
		best_idx = -1; //라인으로 추정되는 객체의 인덱스
		min_dist = DBL_MAX; //최소 거리 저장용 (초기값은 최대로 석정)
		center_x = binary_half.cols / 2.0; // 이미지의 가로 중심 좌
		if (prev_x != -1.0) {//while문이 두번째 실행될때부터 실행하는 조건
			target_x = prev_x;
		}
		else { target_x = center_x; }//while문이 처음 돌때 화면 중앙을 저장하는 조건
		for (int i = 1; i < cnt; i++)
		{
			auto p = stats.ptr<int>(i);//auto를 사용해서 포인터 값이 바뀌어도 자동으로 계산및 수정하게 바꾼코드 원본: int* p
			if (p[4] < 100 || p[4] > 7000) continue;//노이즈 제거(무시)

			double cx = centroids.at<double>(i, 0);//진짜 라인의 무게 중심정보
			double dist = abs(target_x - cx); //화면 중앙과의 거리 계산
			if (dist < min_dist) {
				min_dist = dist;
				best_idx = i;
			}
			auto q = stats.ptr<int>(best_idx);
			if (best_idx != -1 && min_dist < MAX_MOVE_THRESHOLD) {
				
				prev_x = centroids.at<double>(best_idx, 0);
				
				rectangle(binary_half, Rect(q[0], q[1], q[2], q[3]), Scalar(0, 0, 255), 2);
				circle(binary_half, Point(centroids.at<double>(best_idx, 0), centroids.at<double>(best_idx, 1)), 2, Scalar(0, 0, 255), -1);

			}
			else {
				rectangle(binary_half, Rect(q[0], q[1], q[2], q[3]), Scalar(255, 0, 0), 2);
				circle(binary_half, Point(centroids.at<double>(best_idx, 0), centroids.at<double>(best_idx, 1)), 2, Scalar(255, 0, 0), -1);
			}
			if (prev_x != -1.0) {
				error = center_x - prev_x;
			}
			else {
				error = 0.0;
			}
		}
		
		
		
		auto endTime = std::chrono::steady_clock::now(); // 종료시간 저장
		float totalTime = std::chrono::duration<float, std::milli>(endTime - startTime).count();
		cout << "Error: " << error << ", time: " << totalTime << "sec" << endl; // 시간차이(msec)출력
		// === 영상처리 로직 끝 ===

		// 결과 출력
		imshow("Original", frame);
		imshow("Binary ROI Result", binary_half);

		// ESC 키(27)를 누르면 종료
		if (waitKey(30) == 'q') break;
	}

	return 0;
}