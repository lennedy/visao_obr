#ifndef FIND_GEOMETRIC_H
#define FIND_GEOMETRIC_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/ChannelFloat32.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Header.h>
#include <string.h>

class FindGeometric{

	std::string pubImageName;					//Variavel onde ficara armazenado o nome do topico publisher de imagem
	std::string pubGeometricPosName; 	//Variavel onde ficara armazenado o nome do topico publisher de posicao
	std::string subImageName;					//Variavel onde ficara armazenado o nome do topico subscriber de imagem

	//criacao do "no" ROS
	ros::NodeHandle nh;

	ros::NodeHandle _nh; // to get the private params

	image_transport::ImageTransport it;

protected:
	image_transport::Publisher pubImage;	//Declaração de publicacao em topico para enviar imagem com figura geometrica identificada
	image_transport::Subscriber subImage;//Declaração de inscricao em topico para adquirir imagem nao tratada
	ros::Publisher pubGeometricPos; //Declaracao de publicacao em topico para enviar vetor de poses das figuras geometricas encontradas
	ros::Subscriber subImageInfo;

public:

	void init();

	FindGeometric();

	void imageCallBack(const sensor_msgs::ImageConstPtr& msg);

	virtual void desenharFiguraGeometrica(cv::Mat &src)const =0 ;
	virtual void encontrarFiguraGeometrica(const cv::Mat &src)=0 ;
	virtual sensor_msgs::PointCloud getPosFiguraGeometrica()const =0;

	inline void setPubImageName(const std::string &s){pubImageName = s;}
	inline void setPubGeometricPosName(const std::string &s){pubGeometricPosName = s;}	
	inline void setSubImageName(const std::string &s){subImageName = s;}

};

#endif
