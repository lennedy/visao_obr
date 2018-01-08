#include <ros/ros.h>
#include "FindRectangle.h"


int main(int argc, char **argv)
{
	//inicializa o ROS
  ros::init(argc, argv, "find_rectangle");

	FindRectangle c;  

	c.setPubImageName("image_out");
	//c.setPubGeometricPosName("/Teste/circles");
	c.setSubImageName("/camera/image_raw");
	c.init();
  ros::spin();
}
