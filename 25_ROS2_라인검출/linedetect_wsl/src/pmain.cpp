#include "linedetect_wsl/pub.hpp"

int main(int argc, char * argv[]){
    rclcpp::init(argc,argv);
    std::string video_path = "/home/linux/ros2_ws/simulation/7_lt_ccw_100rpm_in.mp4";
    auto node = std::make_shared<CamPublisher>(video_path);
    node->run();
    rclcpp::shutdown();
    return 0;
}