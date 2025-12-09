cmake_minimum_required(VERSION 3.16)
project(linedetect_wsl)

# C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(pub src/pub.cpp src/pmain.cpp)
ament_target_dependencies(pub rclcpp std_msgs sensor_msgs cv_bridge OpenCV)

#Install
install(TARGETS
  pub
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(sub src/sub.cpp src/smain.cpp)
ament_target_dependencies(sub rclcpp std_msgs sensor_msgs cv_bridge OpenCV)

#Install
install(TARGETS
  sub
  DESTINATION lib/${PROJECT_NAME}
)
ament_package()