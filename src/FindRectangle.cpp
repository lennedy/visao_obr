#include "FindRectangle.h"

using namespace cv;
using namespace sensor_msgs;

FindRectangle::FindRectangle(){
	
}

const geometry_msgs::Point32 FindRectangle::determinaCentroide(const RotatedRect &rr)const{
	RotatedRect boundingBox = rr;
	Point2f corners[4];
	boundingBox.points(corners);

	geometry_msgs::Point32 p;

}


void FindRectangle::getRectangleCrentroid(const int biggestContourIdx, PointCloud& centroide)const{

	if( contours.size() > 0 ){	
		RotatedRect rr = minAreaRect(contours[biggestContourIdx]);
		geometry_msgs::Point32 centroidePrincipal;
		centroidePrincipal.x = rr.center.x;
		centroidePrincipal.y = rr.center.y;
		centroidePrincipal.z = 0;	
		mudarOrigem(centroidePrincipal);				//muda a origem do lado superior esquerdo para o centro do video

		ChannelFloat32 corners;
		Point2f vertices[4];
		rr.points(vertices);

		corners.name = "4 vertices: (x,y)";
		for (int i=0; i<4; i++){
			//mudarOrigem(vertices[i]);
			corners.values.push_back(vertices[i].x);
			corners.values.push_back(vertices[i].y);
		}

		centroide.channels.push_back(corners);
		centroide.points.push_back(centroidePrincipal);
	}

}

void FindRectangle::desenharFiguraGeometrica(Mat &src)const{

  std::vector<cv::Vec4i> hierarchy;
  float biggestContourArea = 0;
	int biggestContourIdx = -1;
  Mat drawing = src;
//	Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
  /// Draw contours and find biggest contour (if there are other contours in the image, we assume the biggest one is the desired rect)
  // drawing here is only for demonstration!
  for( int i = 0; i< contours.size(); i++ )
  {
		Scalar color = Scalar(0, 100, 0);
		drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );

		float ctArea= contourArea(contours[i]);
		
		if(ctArea > biggestContourArea && ctArea > 2000.0)
		{
			biggestContourArea = ctArea;
			biggestContourIdx = i;
		}
		if(ctArea > 100){
			// compute the rotated bounding rect of the biggest contour! (this is the part that does what you want/need)
			RotatedRect boundingBox = minAreaRect(contours[i]);
			Point2f corners[4];
			boundingBox.points(corners);
			line(drawing, corners[0], corners[1], Scalar(255,255,255));
			line(drawing, corners[1], corners[2], Scalar(255,255,255));
			line(drawing, corners[2], corners[3], Scalar(255,255,255));
			line(drawing, corners[3], corners[0], Scalar(255,255,255));

		}
  }

  if(biggestContourIdx >= 0 ){
	  // one thing to remark: this will compute the OUTER boundary box, so maybe you have to erode/dilate if you want something between the ragged lines
//		ROS_INFO("OME RAPAZ: %f", biggestContourArea);
		RotatedRect boundingBox = minAreaRect(contours[biggestContourIdx]);
		Point2f corners[4];
		boundingBox.points(corners);
		line(drawing, corners[0], corners[1], Scalar(255,0,0));
		line(drawing, corners[1], corners[2], Scalar(255,0,0));
		line(drawing, corners[2], corners[3], Scalar(255,0,0));
		line(drawing, corners[3], corners[0], Scalar(255,0,0));
		circle(drawing, boundingBox.center, 25,Scalar(255,0,0), 10);

	}

	src = drawing;
}

inline void FindRectangle::encontrarFiguraGeometrica(const Mat &src){

	cv::Mat mask;
  cv::threshold(src, mask, 80, 255, CV_THRESH_BINARY_INV); //| CV_THRESH_OTSU

 // find contours (if always so easy to segment as your image, you could just add the black/rect pixels to a vector)
  cv::findContours(mask,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

}

sensor_msgs::PointCloud FindRectangle::getPosFiguraGeometrica()const{

	PointCloud centroidRectang;
	float biggestContourArea;
	int biggestContourIdx=-1;

  for( int i = 0; i< contours.size(); i++ )
  {
		float ctArea= contourArea(contours[i]);

		if(ctArea > biggestContourArea)
		{
			biggestContourArea = ctArea;
			biggestContourIdx = i;
		}
	}

	if(biggestContourIdx >= 0){
		getRectangleCrentroid(biggestContourIdx, centroidRectang);
	}

	return centroidRectang;

}
