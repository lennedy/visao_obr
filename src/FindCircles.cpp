#include "FindCircles.h"

using namespace cv;
using namespace sensor_msgs;


//utilizando a transformada de Hough encontra os circulos presentes na objeto "src"
void FindCircles::acharCirculos(const Mat &src){
	//Os valores numericos presentes na funcao "HoughCircles" devem ser substituidos por variaveis e constantes
	HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/8, 150, 45, 0, 0 );
}

//Desenha um circulo onde foram encontrados circulos
void FindCircles::desenharCirculos(Mat &src)const{
//percorre o vetor para desenhar cada circulo
	for( size_t i = 0; i < circles.size(); i++ )
	{
		//inicializa o objeto "center" com os valores do centro do circulo
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		//inicializa a variavel "radius" com valor do raio
		int radius = cvRound(circles[i][2]);
		// Desenha um pequeno circulo para representar o centro do circulo encontrado
		circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// Desenha um circulo para representar a borda do circulo encontrado
		circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
	}
}


FindCircles::FindCircles():FindGeometric(){
}

PointCloud FindCircles::getPosFiguraGeometrica()const{

	PointCloud pos_circ; 			//Nuvem de pontos que sera retornada
	geometry_msgs::Point32 centroCirc; //O ponto que contera os valores x e y
	ChannelFloat32 channel;		//O channel contera o valor do raio

	pos_circ.header = header;

	channel.name = "raio do circulo";  //adicionando uma explicacao sobre os dados no channel

	//Esse "for" incluira todos os pontos na nuvem de pontos
	//Tambem incluira o valor do raio
	for( size_t i = 0; i < circles.size(); i++ ){
		//Elimina variaveis adicionadas ateriormente ao vetor "values"
		channel.values.clear();
		centroCirc.x = circles[i][0];	//iniciando o centro do circulo com o valor de x
		centroCirc.y = circles[i][1];	//iniciando o centro do circulo com o valor de y
		mudarOrigem(centroCirc);

		channel.values.push_back(circles[i][2]);	//iniciando um valor do "canal" com o raio do circulo
	
		//Adicionando ao final do vetor "points" o valor do centro do circulo
		pos_circ.points.push_back(centroCirc);
		//Adicionando ao final do vetor "channels" o valor do raio do circulo 
		pos_circ.channels.push_back(channel);
	}

	return pos_circ;

}

