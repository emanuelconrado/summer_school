#ifndef CALC_CPP__CALCULATOR_NODE_HPP
#define CALC_CPP__CALCULATOR_NODE_HPP

#include <memory>
#include <mutex>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

#include <std_srvs/srv/trigger.hpp>
#include <std_msgs/msg/float64.hpp>
#include "example_interfaces/srv/add_two_ints.hpp"

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

namespace calc_cpp
{
  class CalculatorNode : public rclcpp_lifecycle::LifecycleNode{
  public:
    explicit CalculatorNode(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());

    ~CalculatorNode() override;

  private:
    CallbackReturn on_configure(const rclcpp_lifecycle::State &);

    CallbackReturn on_activate(const rclcpp_lifecycle::State &state);

    CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state);

    CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state);

    CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state);

    void getParameters();
    void configPubSub();
    void configTimers();
    void configServices();

    rclcpp::Subcription<std_msgs::msg::Float64MultiArray>::ConstSharedPtr sub_op_numbers_;
    void subOpNumbers(const std_msgs::msg::Float64MultiArray &msg);

    rclcpp_lifecycle::LifecylePublisher<std_msgs::msg::Float64>SharedPtr pub_op_result_;
    rclcpp::TimerBase::SharedPtr tmr_op_executor_;
    void tmrOpExeutor();

    rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr srv_op_change_;
    void srvOpChange(const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request, std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response);

    float number1_;
    float number2_;

    int op_code_;

    double hz_;
  };
}




#endif
