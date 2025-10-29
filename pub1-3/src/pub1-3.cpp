#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <memory>
#include <chrono>

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);//Ros2 초기화 작업을 수행
    auto node = std::make_shared<rclcpp::Node>("node_pub1_3");//노드명 make_shared를 가지는 노드 객체 동적 생성
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));//Qos클래스
    auto mypub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",qos_profile );
    //publisher포인터 객체 생성
    geometry_msgs::msg::Twist message;
    char c = 0;
    message.linear.x = 0.0;
    message.angular.z = 0.0;
    rclcpp::WallRate loop_rate(1.0); //반복주파수를 저장하는 객체(단위 Hz)

    while(rclcpp::ok())
    {
        std::cout<<"키보드를 입력하세요: f:전진, b: 후진, i: 좌회전, r: 우회전";
        std::cin>> c;
        switch(c){
            case 'f':
            message.linear.x = 1.0;
            message.angular.z = 0.0;
            break;
            case 'b':
            message.linear.x = -1.0;
            message.angular.z = 0.0;
            break;
            case 'i':
            message.linear.x = 0.0;
            message.angular.z = 1.0;
            break;
            case 'r':
            message.linear.x = 0.0;
            message.angular.z = -1.0;
            break;
            default:
            message.linear.x = 0.0;
            message.angular.z = 0.0;
            break;
        }
        RCLCPP_INFO(node->get_logger(), "Publish: linear.x=%.2f,angular.z=%.2f", message.linear.x, message.angular.z);
        mypub->publish(message);
        //rclcpp::spin_some(node);
        loop_rate.sleep(); //반복주파수에서 남은 시간 만큼 sleep
    }
    rclcpp::shutdown();
    return 0;
}