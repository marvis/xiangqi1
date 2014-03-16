#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

int main(int argc, char ** argv)
{
	if(argc != 3)
	{
		printf("need two images\n");
		return -1;
	}

	IplImage * image0 = cvLoadImage(argv[1], 1);
	IplImage * image1 = cvLoadImage(argv[2], 1);
	if(!image0)
	{
		printf( "first image is invalid\n" );
		return -1;
	}
	if(!image1)
	{
		printf( "second image is invalid\n" );
		return -1;
	}
	if(image0->width != image1->width ||
			image0->height != image1->height ||
			image0->nChannels != image1->nChannels)
	{
		printf("different image size");
		return -1;
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
	cvSaveImage("diffimage.png", diffImg);
	cvReleaseImage(&image0);
	cvReleaseImage(&image1);
	cvReleaseImage(&diffImg);
	return 0;

}
