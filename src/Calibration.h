#ifndef FIND_CALIBRATION_H
#define FIND_CALIBRATION_H

#include <ros/ros.h>
#include <sensor_msgs/Range.h>
#include <string.h>
#include <visao_obr/CaliRect.h>
#include <visao_obr/CaliCirc.h>
#include <visao_obr/CaliGeometricCam.h>

class Calibration{

	private:
		std::string subCalibrationName;
		ros::Subscriber subCalibrationTopic;

		void setCalibrationData(const visao_obr::CaliGeometricCam& msg);

	protected:
		visao_obr::CaliRect caliRect;
		visao_obr::CaliCirc caliCirc;

		

	public:
		Calibration();
		void init(ros::NodeHandle &n);
		inline void setSubCalibrationName(const std::string &s){subCalibrationName = s;}

};

#endif
