#include "FindGeometric.h"

using namespace cv;
using namespace sensor_msgs;

FindGeometric::FindGeometric():it(nh){

	pubImageName 				= "treated_image";
	subImageName 				= "/camera/image_raw";
	pubGeometricPosName	= "circles";

}

void FindGeometric::init(){

	//Iniciando o publicador pubImage
	pubImage = it.advertise(pubImageName.c_str(),1);

	//Iniciando o inscritor subImage
	subImage = it.subscribe(subImageName.c_str(), 1, &FindGeometric::imageCallBack, this);

	//Iniciando o publicador pubGeometricPose
	pubGeometricPos = nh.advertise<sensor_msgs::PointCloud>(pubGeometricPosName.c_str(), 1000);

}

void FindGeometric::imageCallBack(const sensor_msgs::ImageConstPtr& msg){

	//Utiliza-se a classe CvImage para converter objetos/tipos "ROS" para objetos/tipos "openCv"
	cv_bridge::CvImage cvImage; 

	//		setCabecarioFigGeometrica(imageHeader);

	try
	{
		//Faz-se uma copia da imagem do objeto "ROS" msg para o cvImage	
		cvImage =  *cv_bridge::toCvCopy(msg, image_encodings::MONO8) ;

		//Faz a identificacao da posicao de cada figura geometrica
		encontrarFiguraGeometrica(cvImage.image);

		//Desenha cada circulo encontrado na imagem cvImage
		desenharFiguraGeometrica(cvImage.image);

		//Publica-se a imagem depois de desenhar cada figura geometrica
		pubImage.publish(cvImage.toImageMsg());
	}
	catch (cv_bridge::Exception& e)
	{
		//imprime o erro se este vier a ocorrer
		ROS_ERROR("Could not convert ros to openCV. '%s'.", e.what());
	}
	//publica o valor de posicao de cada figura geometrica
//	pubGeometricPos.publish(getPosFiguraGeometrica());


}


