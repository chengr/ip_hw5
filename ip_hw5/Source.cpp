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
	int lowThreshold=150;
	int const max_lowThreshold =450;
	int ratio = 3;
	int kernel_size = 3;

//
	String ins="test3";
	/// Read the image 
	src = imread(ins+".jpg", CV_RGB2GRAY  );
	result = imread(ins+".jpg", 1  );
	int row=src.rows;
	int col=src.cols;
	Mat hps(row*3,360 , CV_8UC1, Scalar(0,0,0));

	int  mid=row*3/2;
	  /// Reduce noise with a kernel 3x3
	//blur( src, src, Size(3,3) );

	  /// Canny detector
	Canny( src, src, lowThreshold, lowThreshold*ratio, kernel_size );
	int xx=col/2;
	int yy=row/2;
	//array save
	int **arr_hps;//BGR
	arr_hps = new int *[360];
	for(int i=0;i<360;i++){
		arr_hps[i]=new int [row*3];
	}
	for(int i=0;i<360;i++)
		for(int j=0;j<row*3;j++){
			arr_hps[i][j]=0;
	}


	//draw hough parameter space
	for(int i=0;i<col;i++){//x
		for(int j=0;j<row;j++){//y
				int tmp=src.at<uchar>(Point(i,j));
				if(tmp==255){
					for(int deg=1;deg<=360;deg++){
						double rad=((double)deg*M_PI)/180;//rad=deg*(pi/180)
						double tho=(double)i*cos(rad)+(double)j*sin(rad);
						//cout<<"deg:"<<deg<<endl;
						//cout<<"tho:"<<tho+xx<<endl;
						//cout<<"deg:"<<deg<<endl;
						if(tho+mid<row*3&&tho+mid>=0){
							arr_hps[deg-1][(int)tho+mid]++;
							//cout<<"deg"<<arr_hps[deg-1][(int)tho+mid]<<endl;
							if(hps.at<uchar>(Point(deg-1,tho+mid))<255)
								hps.at<uchar>(Point(deg-1,tho+mid))++;
						}
					}
				}
		}
	}
	int big_val=0;
	int tho=0;
	int theata=0;
	for(int i=0;i<hps.cols;i++){
		for(int j=0;j<hps.rows;j++){
			//int tmp=hps.at<uchar>(Point(i,j));
			int tmp=arr_hps[i][j];
			if(tmp>big_val){
				tho=j-mid;
				theata=i;
				big_val=tmp;
			}
		}
	}
	
	cout<<"tho:"<<tho<<endl;
	cout<<"theata:"<<theata<<endl;
	cout<<"val:"<<big_val<<endl;
	cout<<"mid:"<<mid<<endl;
	//
	int lst=99999;
	int ly=0;
	int rst=-100;
	int ry=0;
	for(int i=0;i<col;i++){
		for(int j=0;j<row;j++){
			int tmp=src.at<uchar>(Point(i,j));
			if(tmp==255){
				double rad=((double)theata*M_PI)/180;
				double tho_in=(double)(i)*cos(rad)+(double)(j)*sin(rad);
				if((int)tho_in==tho){
					Vec3b color;
					color[2]=255;
					color[1]=0;
					result.at<Vec3b>(Point(i,j))=color;
					if(i<lst){
						lst=i;
						ly=j;
					}
					if(i>rst){
						rst=i;
						ry=j;
					}
				}
			}
		}
	}
	/*
	double thea=(theata*M_PI)/180;
	int y=(tho)/sin(thea);
	int x=(tho)/cos(thea);
	cout<<"l:"<<lst<<endl;
	cout<<"r:"<<rst<<endl;*/
	Point FromPoint1 = cvPoint(lst,ly);
    Point ToPoint1 = cvPoint(rst,ry);

    Scalar Color=CV_RGB(255,0,0);
	line(result,FromPoint1,ToPoint1,Color,1,CV_AA);
	/*
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
	*/
	
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