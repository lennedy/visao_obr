#ifndef FIND_RECTANGLE_H
#define FIND_RECTANGLE_H

#include "FindGeometric.h"

class FindRectangle:public FindGeometric{

private:
  std::vector<std::vector <cv::Point> > contours;

	const geometry_msgs::Point32 determinaCentroide(const cv::RotatedRect &rr)const;

public:
	FindRectangle();
	void desenharFiguraGeometrica(cv::Mat &src)const;
	void encontrarFiguraGeometrica(const cv::Mat &src);
	sensor_msgs::PointCloud getPosFiguraGeometrica()const;


};

#endif
