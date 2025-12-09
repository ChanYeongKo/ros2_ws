#include "linedetect_wsl/sub.hpp"

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LineSubscriber>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
