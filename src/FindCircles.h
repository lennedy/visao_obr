#ifndef FIND_CIRCLES_H
#define FIND_CIRCLES_H

#include "FindGeometric.h"

//Quando o minimo ou maximo do raio forem 0 significa que nao minimo ou maximo
#define MINIMO_RAIO	0
#define MAXIMO_RAIO 0

class FindCircles: public FindGeometric{

private:

	std::vector<cv::Vec3f> circles; //Declaracao de vetor que contera o centro e os raios dos circulos
	void acharCirculos(const cv::Mat &src);
	void desenharCirculos(cv::Mat &src)const;
	

protected:


public:

	FindCircles();
	
	inline void desenharFiguraGeometrica(cv::Mat &src)const{desenharCirculos(src);}
	inline void encontrarFiguraGeometrica(const cv::Mat &src){acharCirculos(src);}
	sensor_msgs::PointCloud getPosFiguraGeometrica()const;
		

};

#endif
