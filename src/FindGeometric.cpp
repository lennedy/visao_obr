#include "FindGeometric.h"

using namespace cv;
using namespace sensor_msgs;

FindGeometric::FindGeometric():it(nh),_nh("~"){

	pubImageName 				= "treated_image";
	subImageName 				= "/camera/image_raw";
	pubGeometricPosName	= "circles";

  std::string image_out_topic;
  _nh.param("image_out_topic", image_out_topic, std::string("image_out_topic"));
	setPubImageName(image_out_topic);

  std::string position_out_topic;
  _nh.param("position_out_topic", position_out_topic, std::string("position_out_topic"));
	setPubGeometricPosName(position_out_topic);

  std::string centerPosition_out_topic;
  _nh.param("centerPosition_out_topic", centerPosition_out_topic, std::string("centerPosition_out_topic"));
	setPubGeometricCenterPosName(centerPosition_out_topic);


  std::string image_in_topic;
  _nh.param("image_in_topic", image_in_topic, std::string("image_in_topic"));
	setSubImageName(image_in_topic);

  std::string cali_in_topic;
  _nh.param("cali_in_topic", cali_in_topic, std::string("cali_in_topic"));
	setSubCalibrationName(cali_in_topic);

}

void FindGeometric::init(){

	//Iniciando o publicador pubImage
	pubImage = it.advertise(pubImageName.c_str(),1);

	//Iniciando o inscritor subImage
	subImage = it.subscribe(subImageName.c_str(), 1, &FindGeometric::imageCallBack, this);

	//Iniciando o publicador pubGeometricPos
	pubGeometricPos = nh.advertise<sensor_msgs::PointCloud>(pubGeometricPosName.c_str(), 1000);

	//Iniciando o publicador pubGeometricCenterPos
	pubGeometricCenterPos = nh.advertise<geometry_msgs::Point32>(pubGeometricCenterPosName.c_str(), 1000);

	
	Calibration::init(nh);

}

void FindGeometric::imageCallBack(const sensor_msgs::ImageConstPtr& msg){

	//Utiliza-se a classe CvImage para converter objetos/tipos "ROS" para objetos/tipos "openCv"
	cv_bridge::CvImage cvImage_rgb, cvImage_grey;

	camInfo.height	= msg->height;
	camInfo.width		= msg->width;
	header 					= msg->header;

	try
	{
		//Faz-se uma copia da imagem do objeto "ROS" msg para o cvImage	
		cvImage_grey =  *cv_bridge::toCvCopy(msg, image_encodings::MONO8) ;
    cvImage_rgb = *cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

		//Faz a identificacao da posicao de cada figura geometrica
		encontrarFiguraGeometrica(cvImage_grey.image);

		//Desenha cada circulo encontrado na imagem cvImage
		desenharFiguraGeometrica(cvImage_rgb.image);
		
		//Publica-se a imagem depois de desenhar cada figura geometrica
		pubImage.publish(cvImage_rgb.toImageMsg());

	}
	catch (cv_bridge::Exception& e)
	{
		//imprime o erro se este vier a ocorrer
		ROS_ERROR("Could not convert ros to openCV. '%s'.", e.what());
	}

	PointCloud geometricPos = getPosFiguraGeometrica();
	//publica o valor de posicao de cada figura geometrica
	pubGeometricPos.publish(geometricPos);

	//p32	= geometriPos.points[0];
	if(geometricPos.points.size()>0)
		pubGeometricCenterPos.publish(geometricPos.points[0]);

}

void FindGeometric::mudarOrigem(geometry_msgs::Point32 &point)const{


	point.x = point.x - camInfo.width/2.0;
	point.y = camInfo.height/2.0 - point.y;

}
