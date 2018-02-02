#include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <visao_obr/TutorialsConfig.h>

void callback(dynamic_tutorials::TutorialsConfig &config, uint32_t level) {
//  ROS_INFO("Reconfigure Request: %d %d %d %s %d", 
//            config.cali_rect, config.cali_circ_canny, 
//            config.cali_circ_hough, 
//            config.bool_param?"True":"False", 
//            config.size);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "CalibrationServer");

  dynamic_reconfigure::Server<dynamic_tutorials::TutorialsConfig> server;
  dynamic_reconfigure::Server<dynamic_tutorials::TutorialsConfig>::CallbackType f;

  f = boost::bind(&callback, _1, _2);
  server.setCallback(f);

  ROS_INFO("Spinning node");
  ros::spin();
  return 0;
}
