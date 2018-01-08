#include <ros/ros.h>
#include "FindRectangle.h"


int main(int argc, char **argv)
{
	//inicializa o ROS
  ros::init(argc, argv, "find_rectangle");

	FindRectangle c;  

	c.init();
  ros::spin();
}
