#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
	cout<<x<<" "<<y<<" "<<width<<" "<<height<<endl;
	cvSetImageROI(src, cvRect(x, y, width , height));
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvCopy(src, dst, 0);
    cvResetImageROI(src);
	return dst;
}

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		printf("No image data \n");
		return -1;
	}

	string filename = argv[1];
	IplImage * image = cvLoadImage(argv[1], 1);
	if(!image)
	{
		printf( "No image data \n" );
		return -1;
	}

	// crop start 
	int x = 4;
	int y = 149;
	int width = 531;
	int height = 660;
	IplImage * cropImg = cropImage(image, x, y, width, height);
	cvReleaseImage(&image);
	image = cropImg;
	// crop end

	// convert to gray start
	IplImage * grayImg = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
	cvCvtColor(image,grayImg,CV_BGR2GRAY);
	// convert to gray end
	
	// draw start
	int x0 = 29;
	int y0 = 29;
	int x_step = 59;
	int y_step = 66;
	int id = 1;
	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			int w = 29;
			int x = x0 + i * x_step - w;
			int y = y0 + j * y_step - w;
			IplImage * chessImg = cropImage(grayImg, x, y, 2*w+1, 2*w+1);
			ostringstream oss;
			oss << filename<< "."<< id++ << ".png";
			cvSaveImage(oss.str().c_str(), chessImg);
			cvReleaseImage(&chessImg);
		}
	}
	// draw end
	
	cvReleaseImage(&image);
	cvReleaseImage(&grayImg);
	return 0;

}
