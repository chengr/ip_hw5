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
Mat src,img, gray_image;
double avg(int x,int y);
double avg2(int x,int y,double in);
int checks2(int x,int y);
int checks(int x,int y);
/** @function main */
int main(int argc, char** argv)
{
	int var=20;
	String ins="F16";
	/// Read the image 
	src = imread(ins+".jpg", 1 );
	cvtColor( src, gray_image, CV_RGB2GRAY );
	cvtColor( src, img, CV_RGB2GRAY );
	cvtColor( src, src, CV_RGB2GRAY );
	int row=src.rows;
	int col=src.cols;
	int np=2;
	int sum=(col*row)/np;


	const int nrolls=sum;  // number of experiments

	std::default_random_engine generator(time(0));
	std::normal_distribution<double> distribution(127.0,var);
	int p[256]={};
	int *noise_v;
	noise_v=new int[sum];
	int count=0;
	for (int i=0; i<nrolls; ++i) {
		double number = distribution(generator);
		if ((number>=0.0)&&(number<255.0)){
			++p[int(number)];
			noise_v[i]=number-127;
		}
	}
	for (int i=0; i<256; ++i) {
		cout << i << "-" << (i+1) << ": ";
		cout << p[i]<<endl;
		count+=p[i];
	}
	cout << "count:"<<count<< endl;
	// 亂數排序
	for(int i = 0,j,temp ; i < nrolls ; ++i) {
		j=rand()%nrolls;
		temp=noise_v[j];
		noise_v[j]=noise_v[i];
		noise_v[i]=temp;
	}
	for(int i = 0,j,temp ; i < nrolls ; ++i) {
		j=rand()%nrolls;
		temp=noise_v[j];
		noise_v[j]=noise_v[i];
		noise_v[i]=temp;
	}

	unsigned seed;
    seed = (unsigned)time(NULL); // 取得時間序列
    srand(seed);

	int v=0;
	for(int i=0;i<col;i++){
		for(int j=0;j<row;j++){
			
			//	uchar color=noise_v[v];
				//cout << "N:"<<noise_v[v]<< endl;
				uchar color2=src.at<uchar>(Point(i,j));
				//src.at<uchar>(Point(i,j))= color;
				if(rand()%np==1){
					int tmp= noise_v[v]+color2;
					if(tmp>255)
						tmp=255;
					if(tmp<0)
						tmp=0;
					src.at<uchar>(Point(i,j))=tmp;
				}
				v++;
				if(v>sum){
					v=0;
				}
		}
	}
	namedWindow( "gray", CV_WINDOW_AUTOSIZE );
	imshow( "gray", src );
	imwrite( ins+"_noise.jpg", src );


	for(int i=0;i<col;i++){
		for(int j=0;j<row;j++){
			
				uchar g=src.at<uchar>(Point(i,j));
				
				double n=var;
				double ml= avg(i,j);
				double l=avg2(i,j,ml);
				double co=(n*n)/(l*l);
				if(co>1){
					co=1;
				}
				uchar f=g-co*(g-ml);
				gray_image.at<uchar>(Point(i,j))= f;
		}
	}
	namedWindow( "result", CV_WINDOW_AUTOSIZE );
	imshow( "result", gray_image );
	imwrite( ins+"_result.jpg", gray_image );

	for(int i=0;i<col;i++){
		for(int j=0;j<row;j++){
				uchar f=avg(i,j);
				img.at<uchar>(Point(i,j))= f;
		}
	}
	namedWindow( "avg", CV_WINDOW_AUTOSIZE );
	imshow( "avg", img );
	imwrite( ins+"_avg.jpg", img );

	delete []noise_v;
	waitKey(0);
	return 0;
}
//
double avg(int x,int y){
	int lu=checks2(x-1,y-1);
	int u=checks2(x,y-1);
	int ru=checks2(x+1,y-1);
	int l=checks2(x-1,y);
	int m=checks2(x,y);
	int r=checks2(x+1,y);
	int ld=checks2(x-1,y+1);
	int d=checks2(x,y+1);
	int rd=checks2(x+1,y+1);
	double color_v=(lu+u+ru+l+m+r+ld+d+rd)/9;
	return color_v;
}
double avg2(int x,int y,double in){
	int lu=checks2(x-1,y-1);
	int u=checks2(x,y-1);
	int ru=checks2(x+1,y-1);
	int l=checks2(x-1,y);
	int m=checks2(x,y);
	int r=checks2(x+1,y);
	int ld=checks2(x-1,y+1);
	int d=checks2(x,y+1);
	int rd=checks2(x+1,y+1);



	double color_v=sqrt((
		((double)lu-in)*((double)lu-in)+
		((double)u-in)*((double)u-in)+
		((double)ru-in)*((double)ru-in)+
		((double)l-in)*((double)l-in)+
		((double)m-in)*((double)m-in)+
		((double)r-in)*((double)r-in)+
		((double)ld-in)*((double)ld-in)+
		((double)d-in)*((double)d-in)+
		((double)rd-in)*((double)rd-in)
	)/9);
	return color_v;
}


int checks(int x,int y){
	if(x>=0&&x<src.cols&&y>=0&&y<src.rows){
		uchar c=src.at<uchar>(y,x);
		return 	c;
	}
	else
		return 0;
}

int checks2(int x,int y){
	if(x>=0&&x<src.cols&&y>=0&&y<src.rows){
		uchar c=gray_image.at<uchar>(y,x);
		return 	c;
	}
	else
		return 0;
}
/*

				uchar color=number;
				//src.at<uchar>(Point(i,j))= color;
				if(rand()%2==1){
					src.at<uchar>(Point(i,j))= color;
				}


*/