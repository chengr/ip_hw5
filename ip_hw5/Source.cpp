#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cv.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <random>
#include <time.h>
# define M_PI           3.14159265358979323846  /* pi */
using namespace std;
using namespace cv;
Mat src,hps,result;
/** @function main */
int main(int argc, char** argv)
{
	int edgeThresh = 1;
	int lowThreshold=50;
	int const max_lowThreshold = 150;
	int ratio = 3;
	int kernel_size = 3;

//
	String ins="map2";
	/// Read the image 
	src = imread(ins+".jpg", CV_RGB2GRAY  );
	result = imread(ins+".jpg", 1  );
	int row=src.rows;
	int col=src.cols;
	Mat hps(row*2,360 , CV_8UC1, Scalar(0,0,0));


	  /// Reduce noise with a kernel 3x3
	blur( src, src, Size(3,3) );

	  /// Canny detector
	Canny( src, src, lowThreshold, lowThreshold*ratio, kernel_size );
	int xx=col/2;
	int yy=row/2;
	//draw hough transform space
	for(int i=0;i<col;i++){//x
		for(int j=0;j<row;j++){//y
				int tmp=src.at<uchar>(Point(i,j));
				if(tmp==255){
					for(int deg=1;deg<=360;deg++){
						double rad=(deg*M_PI)/180;//rad=deg*(pi/180)
						double tho=(i-xx)*cos(rad)+(j-yy)*sin(rad);
						//cout<<"deg:"<<deg<<endl;
						//cout<<"tho:"<<tho+xx<<endl;
						//cout<<"deg:"<<deg<<endl;
						if(tho+col<row*2)
							if(hps.at<uchar>(Point(deg-1,tho+col))<255)
								hps.at<uchar>(Point(deg-1,tho+col))++;
					}
				}
		}
	}
	int big_val=0;
	int tho=0;
	int theata=0;
	for(int i=0;i<hps.cols;i++){
		for(int j=0;j<hps.rows;j++){
			int tmp=hps.at<uchar>(Point(i,j));
			if(tmp>=big_val){
				tho=j;
				theata=i;
				big_val=tmp;
			}
		}
	}
	
	cout<<"tho:"<<tho<<endl;
	cout<<"theata:"<<theata<<endl;
	cout<<"val:"<<big_val<<endl;

	//
	
	for(int i=0;i<col;i++){
		for(int j=0;j<row;j++){
			int tmp=src.at<uchar>(Point(i,j));
			if(tmp==255){
				double thea=(theata*M_PI)/180;
				double tho_in=(i-xx)*cos(thea)+(j-yy)*sin(thea);
				if((int)tho_in==tho-col){
					Vec3b color;
					color[0]=255;
					color[1]=0;
					result.at<Vec3b>(Point(i,j))=color;
				}
			}
		}
	}
	
	for(int i=0;i<col;i++){
		double thea=(theata*M_PI)/180;
		int y=((tho-col)-((i-xx)*cos(thea)))/sin(thea)-yy;
		Vec3b color;
		color[0]=255;
		color[1]=0;
		if(y<row&&y>=0){
			result.at<Vec3b>(Point(i,y))=color;
			cout<<"Y:"<<y<<endl;
		}
	}
	
	
	namedWindow( "canny", CV_WINDOW_AUTOSIZE );
	imshow( "canny", src );
	imwrite( ins+"_canny.jpg", src);

	namedWindow( "hps", CV_WINDOW_AUTOSIZE );
	imshow( "hps",hps );
	imwrite( ins+"_hps.jpg",hps);

	namedWindow( "result", CV_WINDOW_AUTOSIZE );
	imshow( "result",result );
	imwrite( ins+"_result.jpg",result);
	waitKey(0);
	return 0;
}

/*

				uchar color=number;
				//src.at<uchar>(Point(i,j))= color;
				if(rand()%2==1){
					src.at<uchar>(Point(i,j))= color;
				}


*/