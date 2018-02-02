#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/ChannelFloat32.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Header.h>
#include <string.h>

#include <opencv2/highgui/highgui.hpp>
#include <visao_obr/CaliGeometricCam.h>


#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Point.h"
#include <sstream>



using namespace cv;
using namespace visao_obr;
using namespace sensor_msgs;

struct SharedData{
	Rect button;
	Mat3b canvas;	
	ros::Publisher* pubDataSave;
};

class IntefaceGrafica{

	int top_slider;
	int top_slider2;
	SharedData d;

		static void callBackFunc(int event, int x, int y, int flags, void* userdata){
			SharedData *l= (SharedData*)userdata;
	
		  if (event == EVENT_LBUTTONDOWN)
		  {
		      if (l->button.contains(Point(x, y)))
		      {
		          ROS_INFO( "Clicked! X:%d ; Y:%d", x, y);
		          rectangle(l->canvas, l->button, Scalar(0,0,255), 2);
							l->pubDataSave->publish(std_msgs::Empty());
		      }
		  }
		  if (event == EVENT_LBUTTONUP)
		  {
		      rectangle(l->canvas, l->button, Scalar(200, 200, 200), 2);
		  }
		}
		
	public:
		IntefaceGrafica(){
			top_slider=255;
			top_slider2=255;
	    cvNamedWindow("TRACKING COLOR", 2 );
		  cvCreateTrackbar("t1:","TRACKING COLOR",&top_slider, 255);
		  cvCreateTrackbar("t2:","TRACKING COLOR",&top_slider2, 255);
//    	cvStartWindowThread();			
		}

		void init(ros::Publisher* dataSave){
						d.pubDataSave = dataSave;
				    setMouseCallback("TRACKING COLOR", callBackFunc, &d);
		}

		~IntefaceGrafica(){
			cvDestroyWindow("TRACKING COLOR");
		}

		
		void updateImage(const Mat& src){
			Mat img(src.rows, src.cols,  CV_8UC3, Scalar(0, 255, 0));

			String buttonText("Salvar Dados");


			src.copyTo(img(cv::Rect(0,0,src.cols, src.rows)));

	    d.button = Rect(0, 0, img.cols, 100);
		  d.canvas = Mat3b(img.rows + d.button.height, img.cols, Vec3b(0,0,0));			
		  // Draw the button
		  d.canvas(d.button) = Vec3b(200,200,200);
		  putText(d.canvas(d.button), buttonText, Point(d.button.width*0.20, d.button.height*0.7), FONT_HERSHEY_COMPLEX, 2, Scalar(0,0,0));

//			ROS_INFO("src: %d; img: %d button: %d", src.rows, img.rows, button.height);
		  // Draw the image
		  img.copyTo( d.canvas( Rect(0,  d.button.height, img.cols, img.rows)));

			cv::imshow("TRACKING COLOR", d.canvas);
	    cv::waitKey(3);		

		}

		int getSlider1(){return top_slider;}
		int getSlider2(){return top_slider2;}

};


class CaliClient{
	private:
		String subImageName;
		String pubCaliDataName;
		String pubDataSaveName;
		ros::NodeHandle nh;
		image_transport::ImageTransport it;
		image_transport::Subscriber subImage;

	  ros::Publisher pubCaliData;
	  ros::Publisher pubDataSave;

		
		IntefaceGrafica gui;

	public:
		CaliClient():it(nh){}

		void init(){
			//Iniciando o inscritor subImage
			subImage = it.subscribe(subImageName.c_str(), 1, &CaliClient::imageCallBack, this);
			pubCaliData = nh.advertise<visao_obr::CaliGeometricCam>(pubCaliDataName.c_str(), 1000);
			pubDataSave = nh.advertise<std_msgs::Empty>(pubDataSaveName.c_str(), 1000);
			gui.init(&pubDataSave);
		}

		void imageCallBack(const sensor_msgs::ImageConstPtr& msg){

			//Utiliza-se a classe CvImage para converter objetos/tipos "ROS" para objetos/tipos "openCv"
			cv_bridge::CvImagePtr img_ptr;
			//Objeto do tipo opencv para guardar a imagem obtida
    	Mat img_rgb;

			try
			{
				//Faz-se uma copia da imagem do objeto "ROS" msg para o cvImage	
	       img_ptr = cv_bridge::toCvCopy(msg,
	       sensor_msgs::image_encodings::BGR8);
	       img_rgb = img_ptr->image;
			}
			catch (cv_bridge::Exception& e)
			{
				//imprime o erro se este vier a ocorrer
				ROS_ERROR("Could not convert ros to openCV. '%s'.", e.what() );
			}

			gui.updateImage(img_rgb);
		}

		void execute(){
			visao_obr::CaliGeometricCam p;
			
			p.caliRect.limiarDefinicao = gui.getSlider1();
			pubCaliData.publish(p);
//			ROS_INFO("SFDADSFA");

		}

		void setSubImageName(const String& s){
			subImageName = s;
		}
		
		void setPubCaliDataName(const String& s){
			pubCaliDataName = s;
		}

		void setPubDataSaveName(const String& s){
			pubDataSaveName = s;
		}
};


int main(int argc, char **argv){
	//inicializa o ROS
  ros::init(argc, argv, "cali_geometric");

	CaliClient c;

	c.setSubImageName("/PDI/image_out_topic");
	c.setPubCaliDataName("/PDI/cali_in_topic");
	c.setPubDataSaveName("/PDI/save_data");

	c.init();

  ros::Rate loop_rate(10);

  while (ros::ok())
  {

  
		c.execute();
  
    ros::spinOnce();

    loop_rate.sleep();
}


	
}
