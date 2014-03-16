//compile: g++ -mmacosx-version-min=10.8 test.cpp $OPENCV_LIBS

#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

int main1( int argc, char** argv )
{
	Mat image; //定义一个文件

	cout<<argv[1]<<endl;
	image = imread( argv[1], 1); //读取要显示的图片， "1"为默认的值；

	if(argc !=2 || !image.data ) //判断有没有图片，如果没有就显示错误；
	{
		printf( "No image data \n" );
		return -1;
	}
	//cvRectangle((CvArr*)&image, cvPoint(10,10), cvPoint(60,60), cvScalar(255));
	namedWindow("Display Image",CV_WINDOW_AUTOSIZE );
	imshow("Display Image",image );
	waitKey(0);
	return 0;
}

IplImage * cropImage(IplImage * image, int x, int y, int width, int height)
{
	cvSetImageROI(image, cvRect(x, y, width , height));
	IplImage * cropImg = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , image->nChannels);
	cvCopy(image, cropImg, 0);
    cvResetImageROI(image);
	return cropImg;
}

int main(int argc, char ** argv)
{
	IplImage * image = cvLoadImage(argv[1], 1);
	if(!image)
	{
		printf( "No image data \n" );
		return -1;
	}

	// crop start 
	int x = 0;
	int y = 148;
	int width = image->width;
	int height = 660;
	IplImage * cropImg = cropImage(image, x, y, width, height);
	cvReleaseImage(&image);
	image = cropImg;
	// crop end

	// convert to gray start
	IplImage * grayImg = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
	cvCvtColor(image,grayImg,CV_BGR2GRAY);
	// convert to gray end

	// edge image start
	IplImage * edgeImg = cvCreateImage(cvSize(image->width, image->height), IPL_DEPTH_8U, 1);
	cvSobel(grayImg, edgeImg,1,1,3);
	// edge image end
	
	// draw start
	int x0 = 33;
	int y0 = 30;
	int x_step = 59;
	int y_step = 66;
	vector<pair<int, int> > templ;
	for(int x = 0; x <= 29; x++)
	{
		for(int y = 0; y <= 29; y++)
		{
			double dist = x*x + y*y;
			if(dist >= 24*24 && dist <= 29*29)
				templ.push_back(pair<int,int>(x,y));
		}
	}

	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			int w0 = 30;
			int x1 = x0 + i * x_step - w0;
			int y1 = y0 + j * y_step - w0;
			int x2 = x1 + 2*w0;
			int y2 = y1 + 2*w0;
			cvRectangle(grayImg, cvPoint(x1,y1), cvPoint(x2,y2), cvScalar(255));

			/*
			x1 = x0 + i * x_step;
			y1 = y0 + j * y_step;
			double sum = 0;
			double ssum = 0;
			int n = templ.size();
			for(int p = 0; p < n; p++)
			{
				int xx = templ[p].first + x1;
				int yy = templ[p].second + y1;
				double val = grayImg->imageData[xx + yy * grayImg->widthStep];
				sum += val;
				ssum += val*val;
			}
			double avg = sum/n;
			double var = ssum/n - avg*avg;
			cout<<var<<"\t";
			if(var > 2000)
			{
				cvCircle(grayImg, cvPoint(x1,y1), 29, cvScalar(255));
				//cvCircle(grayImg, cvPoint(x1,y1), 24, cvScalar(0));
			}
			*/
		}
	}
	// draw end
	
	cvNamedWindow("Orig Image",CV_WINDOW_AUTOSIZE );
	cvShowImage("Orig Image",grayImg );
	//cvNamedWindow("Edge Image",CV_WINDOW_AUTOSIZE );
	//cvShowImage("Edge Image", edgeImg);
	cvReleaseImage(&image);
	cvReleaseImage(&edgeImg);
	cvReleaseImage(&grayImg);
	waitKey(0);
	return 0;

}
