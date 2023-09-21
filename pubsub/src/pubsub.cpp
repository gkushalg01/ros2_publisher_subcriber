#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cstdlib>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class cPubSub : public rclcpp::Node
{
  public:
    cPubSub()
    : Node("observer"), count_(0)
    {
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
      timer_ = this->create_wall_timer(500ms, std::bind(&cPubSub::publisher_callback, this));
      subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("cmd_vel", 10, std::bind(&cPubSub::subcriber_callback, this, _1));
    }

  private:
    void publisher_callback()
    {
      auto message = geometry_msgs::msg::Twist();
      message.linear.x = rand()%100;
      message.linear.y = rand()%100;
      message.linear.z = rand()%100;
      message.angular.x = rand()%100;
      message.angular.y = rand()%100;
      message.angular.z = rand()%100;
      RCLCPP_INFO(this->get_logger(), "Publishing: '%d'\n linear {x: %f, y: %f, z: %f}; \n angular {x: %f, y: %f, z: %f};", 
      ++count_, message.linear.x, message.linear.y, message.linear.z, message.angular.x, message.angular.y, message.angular.z);
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    size_t count_;

    void subcriber_callback(const geometry_msgs::msg::Twist::SharedPtr message) const
    {
      RCLCPP_INFO(this->get_logger(), "I heard: '%d' \n linear {x: %f, y: %f, z: %f}; \n angular {x: %f, y: %f, z: %f};", 
      count_, message->linear.x, message->linear.y, message->linear.z, message->angular.x, message->angular.y, message->angular.z);
    }
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<cPubSub>());
  rclcpp::shutdown();
  return 0;
}