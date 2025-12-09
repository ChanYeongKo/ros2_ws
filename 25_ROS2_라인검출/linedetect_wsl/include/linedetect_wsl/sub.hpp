#ifndef _SUB_HPP_
#define _SUB_HPP_

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>
#include <iostream>
#include <chrono>

class LineSubscriber : public rclcpp::Node
{
    public:
        LineSubscriber();
    private:
    void callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg);
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr sub_;
    double prev_x_;
};
#endif