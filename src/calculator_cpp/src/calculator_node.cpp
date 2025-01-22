#include "calculator_cpp/calculator_node.hpp"

namespace calc_cpp
{
CalculatorNode::CalculatorNode(const rclcpp::NodeOptions &options) : rclcpp_lifecycle::LifecycleNode("calculator_node", "", options){
  RCLCPP_INFO(get_logger(), "Creating");

  declare_parameter("rate.pub_result", rclcpp:ParameterValue(1.0));


}

CalculatorNode::~CalculatorNode(){}

CallbackReturn on_configure(const rclcpp_lifecycle::State &){
  RCLCPP_INFO(get_logger(), "Configuring");

  getParameters();
  configPubSub();
  configTimers();
  configServices();

  return CallbackReturn::SUCCCESS;
}

void tmrOpExeutor(){
  switch op_code_{
  case 1:
    result_.data = number1_ + number2_;
    break;
  case 2:                                                                                                                                                                                               
    result_.data = number1_ - number2_;                                                                                                                                                                 
    break;
  case 3:                                                                                                                                                                                               
    result_.data = number1_ * number2_;                                                                                                                                                                 
    break;
  case 4:                                                                                                                                                                                               
    result_.data = number1_ / number2_;                                                                                                                                                                 
    break;
  defult:
    result_.data = 0;
  }

  pub_op_result->publish(result);
}

void CalculatorNode::configServices(){
  srv_op_change_ = create_service<example_interfaces::srv::AddTwoInts>("op_change", std::bind(&CalculatorNode::srvOpChange, this, std::placeholders::_1, std::placeholders::_2));
}

void srvOpChange(const std::shared_ptr<example_interfaces::srv::            AddTwoInts::Request> request, std::shared_ptr<example_interfaces::srv::         AddTwoInts::Response> response){
  op_code_ = request->a;
  response->sum = 1;
}

void CalculatorNode::getParameters(){
  get_parameter("rate.pub_result", _hz_);
}

void CalculatorNode::configTimers(){
  tmr_op_executor_ = create_wall_timer(std::chrono::duration<double>(1.0/_hz_), std::bind(&CalculatorNode::tmrOpExecutor, this), nullptr);
}

void Calculator::configPubSub(){
  sub_op_numbers_ = create_subcription<std_msgs::msg::Float64MultiArray>("op_numbers_in", 1, std::bind(&CalculatorNode::subOpNumbers, this, std::placeholders::_1));

  pub_op_numbers = create_publisher<std_msgs::msg::Float64>("op_numbers_out", 1);
}

void CalculatorNode::subOpNumbers(const std_msgs::msg::Float64MultiArray &msg){
  number1_ = msg.data[0];
  number2_ = msg.data[1];
}

CallbackReturn on_activate(const rclcpp_lifecycle::State &state){
  RCLCPP_INFO(get_logger(), "Activating");

  return CallbackReturn::SUCCESS;
}

CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state){
  RCLCPP_INFO(get_logger(), "Deactivating");

  return CallbackReturn::SUCCESS;
}

CallbackReturn on_cleanup(const rclcpp_lifecycle::State &state){
  RCLCPP_INFO(get_logger(), "Cleaning up");

  sub_op_numbers_.reset();

  return CallbackReturn::SUCCESS;
}

CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state){
  RCLCPP_INFO(get_logger(), "Shutting down");

  return CallbackReturn::SUCCESS;
}

}
