#ifndef FIND_CALIBRATION_H
#define FIND_CALIBRATION_H

#include <ros/ros.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/Empty.h>
#include <string.h>
#include <visao_obr/CaliRect.h>
#include <visao_obr/CaliCirc.h>
#include <visao_obr/CaliGeometricCam.h>
#include <dynamic_reconfigure/Config.h>
#include <dynamic_reconfigure/server.h>

class Calibration{

	private:
		std::string subCalibrationName;
		std::string subCalibrationName2;
		
		std::string subSaveCaliDataName;
		ros::Subscriber subCalibrationTopic;
		ros::Subscriber subSaveCaliDataTopic;
		ros::Subscriber subCalibrationTopic2;

		void setCalibrationData(const visao_obr::CaliGeometricCam& msg);
		
		void updateCalibrationData(const dynamic_reconfigure::Config &config);

		void saveCalibrationData(const std_msgs::Empty &msg);
		
	protected:
		visao_obr::CaliRect caliRect;
		visao_obr::CaliCirc caliCirc;

		

	public:
		Calibration();
		void init(ros::NodeHandle &n);
		inline void setSubCalibrationName(const std::string &s){subCalibrationName = s;}
		inline void setSubSaveCaliDataName(const std::string &s){subCalibrationName = s;}

};

#endif
