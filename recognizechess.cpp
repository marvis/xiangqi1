#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
	cvSetImageROI(src, cvRect(x, y, width , height));
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvCopy(src, dst, 0);
    cvResetImageROI(src);
	return dst;
}

double sumImageRoi(IplImage * image, int x, int y, int width, int height)
{
	double sum = 0;
	int nchannels = image->nChannels;
	for(int j = y; j < y+height; j++)
	{
		for(int i = x; i < x+width; i++)
		{
			for(int c = 0; c < image->nChannels; c++)
			{
				sum += image->imageData[i*nchannels + c + j*image->widthStep];
			}
		}
	}
	return sum;
}

IplImage * imageDiff(IplImage * image0, IplImage * image1)
{
	if(!image0)
	{
		printf( "image0 is invalid\n" );
		return 0;
	}
	if(!image1)
	{
		printf( "image1 is invalid\n" );
		return 0;
	}
	if(image0->width != image1->width ||
			image0->height != image1->height ||
			image0->nChannels != image1->nChannels)
	{
		printf("image0 and image1 have different size");
		return 0;
	}

	int width = image0->width;
	int height = image0->height;
	int nchannels = image0->nChannels;
	IplImage * diffImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, nchannels);
	int widthStep = diffImg->widthStep;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int ind = i*nchannels + c + j * widthStep;
				int val0 = image0->imageData[ind];
				int val1 = image1->imageData[ind];
				int diffval = val0 - val1;
				diffval = (diffval > 0) ? diffval : -diffval;
				diffImg->imageData[ind] = diffval;
			}
		}
	}
	return diffImg;
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

	IplImage * bkgImg = cvLoadImage("background.png", 1);
	if(!bkgImg)
	{
		printf( "No background.png in current folder");
		return -1;
	}

	IplImage * image2 = cropImage(image, 4, 149, 531, 660);
	cvReleaseImage(&image);
	image = image2;

	IplImage * bkgImg2 = cropImage(bkgImg, 4, 149, 531, 660);
	cvReleaseImage(&bkgImg);
	bkgImg = bkgImg2;

	IplImage * diffImg = imageDiff(bkgImg, image);
	
	IplImage * grayImg = cvCreateImage(cvSize(diffImg->width, diffImg->height), IPL_DEPTH_8U, 1);
	cvCvtColor(diffImg,grayImg,CV_BGR2GRAY);

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
			int w0 = 29;
			int x = x0 + i * x_step - w0;
			int y = y0 + j * y_step - w0;
			double sumval = sumImageRoi(grayImg, x, y, 2*w0+1, 2*w0+1);
			cout<<sumval<<" ";
			if(sumval > 50000)
			{
				IplImage * chessImg = cropImage(image, x, y, 2*w0+1, 2*w0+1);
				ostringstream oss;
				oss << filename<< ".pos"<< id++ << ".png";
				cvSaveImage(oss.str().c_str(), chessImg);
				cvReleaseImage(&chessImg);
			}
		}
		cout<<endl;
	}
	// draw end
	
	cvReleaseImage(&image);
	cvReleaseImage(&bkgImg);
	cvReleaseImage(&grayImg);
	return 0;

}
