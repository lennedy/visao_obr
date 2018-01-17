#include "Calibration.h"

Calibration::Calibration(){
	subCalibrationName="/PDI/calibrate";
}

void Calibration::setCalibrationData(const visao_obr::CaliGeometricCam& msg){
	caliRect = msg.caliRect;
	caliCirc = msg.caliCirc;
}


void Calibration::init(ros::NodeHandle &n){

	subCalibrationTopic = n.subscribe(subCalibrationName.c_str(), 1000, &Calibration::setCalibrationData, this);
}
