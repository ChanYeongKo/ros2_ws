#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include <memory>
#include <chrono>
#include <functional>
//#include <string>
using namespace std::chrono_literals;
void callback(rclcpp::Node::SharedPtr node,
rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr mypub)
{
auto message = geometry_msgs::msg::Vector3();
std::vector<double> v(3);
std::cout<<"벡터값 3개를 입력하세요";
    for (int i = 0; i < 3; i++)
    {
        std::cin>>v[i];
    }
    message.x = v[0];
    message.y = v[1];
    message.z = v[2];
RCLCPP_INFO(node->get_logger(), "Publish:x = %.2f,Publish:y = %.2f,Publish:z = %.2f", message.x,message.y,message.z);
mypub->publish(message);
}
int main(int argc, char* argv[])
{
rclcpp::init(argc, argv);
auto node = std::make_shared<rclcpp::Node>("mynode");
auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
auto pub = node->create_publisher<geometry_msgs::msg::Vector3>("mytopic", qos_profile
);
std::function<void()> fn = std::bind(callback, node, pub);
auto timer = node->create_wall_timer(100ms, fn);
rclcpp::spin(node);
rclcpp::shutdown();
return 0;
}