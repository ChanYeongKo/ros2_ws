#include "rclcpp/rclcpp.hpp"
#include "dxl3/dxl.hpp"
#include <memory>
#include <chrono>
#include <iostream>
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("dxl");
    rclcpp::WallRate loop_rate(20.0); //반복주파수 20hz
    Dxl mx; //Dxl객체 생성
    if(!mx.open()) //장치열기
    {
        RCLCPP_INFO(node->get_logger(), "dynamixel openerror\n");
        rclcpp::shutdown();
        return -1;
    }
    int leftspeed = 0, rightspeed = 0, direnction = 1;
    while(rclcpp::ok())
    {
        leftspeed += direnction * 10;
        rightspeed += direnction * 10;
        if (leftspeed >= 100 && rightspeed >= 100)
            direnction = -1;   
        else if (leftspeed <= -100 && rightspeed <= -100)
            direnction = 1;    
        mx.setVelocity(leftspeed, rightspeed); //속도명령 전송
        loop_rate.sleep();
    }
    mx.close();
    rclcpp::shutdown();
    return 0;
}
