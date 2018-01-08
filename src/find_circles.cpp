#include <ros/ros.h>
#include "FindCircles.h"


int main(int argc, char **argv)
{
	//inicializa o ROS
  ros::init(argc, argv, "find_circles");

	FindCircles c;  

	c.init();
  ros::spin();
}
