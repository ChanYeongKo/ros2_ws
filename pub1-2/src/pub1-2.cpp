#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include <memory>
#include <chrono>
#include <vector>
using namespace std;
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);//Ros2 초기화 작업을 수행
    auto node = std::make_shared<rclcpp::Node>("node_pub1_2");//노드명 make_shared를 가지는 노드 객체 동적 생성
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));//Qos클래스
    auto mypub = node->create_publisher<geometry_msgs::msg::Vector3>("topic_pub1_2",qos_profile );
    //publisher포인터 객체 생성
    geometry_msgs::msg::Vector3 message;
    vector<double> v(3);
    cout<<"벡터값 3개를 입력하세요";
    for (int i = 0; i < 3; i++)
    {
        cin>>v[i];
    }
    message.x = v[0];
    message.y = v[1];
    message.z = v[2];
    rclcpp::WallRate loop_rate(5.0); //반복주파수를 저장하는 객체(단위 Hz)
    while(rclcpp::ok())
    {
        
        RCLCPP_INFO(node->get_logger(), "Publish: x =%.2f, y=%.2f, z = %.2f", message.x,message.y,message.z);
        mypub->publish(message);
        //rclcpp::spin_some(node);
        loop_rate.sleep(); //반복주파수에서 남은 시간 만큼 sleep
    }
    rclcpp::shutdown();
    return 0;
}