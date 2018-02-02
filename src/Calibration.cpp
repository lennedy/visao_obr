#include "Calibration.h"
#include <fstream>

Calibration::Calibration(){
	subCalibrationName	="/PDI/calibrate";
	subSaveCaliDataName = "/PDI/save_data";
	subCalibrationName2 = "/calibracao/CalibrationServer/parameter_updates";

}

void Calibration::setCalibrationData(const visao_obr::CaliGeometricCam& msg){
//	caliRect = msg.caliRect;
//	caliCirc = msg.caliCirc;
}


void Calibration::updateCalibrationData(const dynamic_reconfigure::Config &config){

	caliRect.limiarDefinicao 	= config.ints[0].value;
	caliCirc.limiarCanny			=	config.ints[1].value;
	caliCirc.limiarHough			=	config.ints[2].value;
}


void Calibration::init(ros::NodeHandle &n){

	subCalibrationTopic 	= n.subscribe(subCalibrationName.c_str(), 1000, &Calibration::setCalibrationData, this);
	subSaveCaliDataTopic 	= n.subscribe(subSaveCaliDataName.c_str(), 1000, &Calibration::saveCalibrationData, this);
	subCalibrationTopic2	= n.subscribe(subCalibrationName2.c_str(), 1000, &Calibration::updateCalibrationData, this);
}


void Calibration::saveCalibrationData(const std_msgs::Empty &msg){
	ROS_INFO("Dados salvos: Retângulo: %d", caliRect.limiarDefinicao);
	ROS_INFO("Dados salvos: Círculo: %d; %d", caliCirc.limiarCanny, caliCirc.limiarHough);

}

