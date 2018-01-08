#include <ros/ros.h>
#include "FindCircles.h"


int main(int argc, char **argv)
{
	//inicializa o ROS
  ros::init(argc, argv, "find_circles");

	FindCircles c;  

	ros::NodeHandle _nh("~"); // to get the private params

  std::string image_out_topic;
  _nh.param("image_out_topic", image_out_topic, std::string("image_out_topic"));
	c.setPubImageName(image_out_topic);

  std::string position_out_topic;
  _nh.param("position_out_topic", position_out_topic, std::string("position_out_topic"));
	c.setPubGeometricPosName(position_out_topic);

  std::string image_in_topic;
  _nh.param("image_in_topic", image_in_topic, std::string("image_in_topic"));
	c.setSubImageName(image_in_topic);

	c.init();
  ros::spin();
}
