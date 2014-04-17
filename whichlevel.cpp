#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include <string>
#include <cassert>
#include "path.h"

using namespace std;
using namespace cv;

#define DOUBLE_INF 1.79e+308

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
	cvSetImageROI(src, cvRect(x, y, width , height));
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvCopy(src, dst, 0);
    cvResetImageROI(src);
	return dst;
}

double sumImgDiff(IplImage * img1, IplImage * img2)
{
	//assert(img1 != NULL && img2 != NULL );
	assert(img1->width == img2->width && img1->height == img2->height && img1->nChannels == img2->nChannels);
	int width = img1->width;
	int height = img1->height;
	int nchannels = img1->nChannels;
	double sumdiff = 0.0;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				double val1 = CV_IMAGE_ELEM(img1, unsigned char , j, i*nchannels+c);
				double val2 = CV_IMAGE_ELEM(img2, unsigned char , j, i*nchannels+c);
				sumdiff += fabs(val1-val2);
			}
		}
	}
	return sumdiff;
}
int main(int argc, char ** argv)
{
	if(argc == 1) return 0;
	IplImage * image = cvLoadImage(argv[1], 1);
	if(!image)
	{
		cerr<<"unable to load image "<<argv[1]<<endl;
		return 0;
	}
	IplImage * digImg = cropImage(image, 312, 375, 25, 24);
	if(!digImg)
	{
		cerr<<"crop error"<<endl;
		return 0;
	}
	int width = digImg->width;
	int height = digImg->height;
	double min_sumdiff = DOUBLE_INF;
	int level=-1;
	for(int i = 0; i < 9; i++)
	{
		char buf[255]; 
		sprintf(buf, XQTMPLPATH"level_prepared_%d.png", i+1);
		IplImage * tmpImg1 = cvLoadImage(buf, 1);
		if(!tmpImg1)
		{
			cerr<<"unable to load image "<<buf<<endl;
			return 0;
		}
		double sumdiff = sumImgDiff(digImg, tmpImg1);
		if(sumdiff < min_sumdiff)
		{
			min_sumdiff = sumdiff;
			level = i+1;
		}
		sprintf(buf, XQTMPLPATH"level_nonprepared_%d.png", i+1);
		//cout<<"\""<<buf<<"\""<<endl;
		IplImage * tmpImg2 = cvLoadImage(buf, 1);
		if(!tmpImg2)
		{
			cerr<<"unable to load image "<<buf<<endl;
			return 0;
		}
		sumdiff = sumImgDiff(digImg, tmpImg2);
		if(sumdiff < min_sumdiff)
		{
			min_sumdiff = sumdiff;
			level = i+1;
		}
		cvReleaseImage(&tmpImg1);
		cvReleaseImage(&tmpImg2);
	}
	cout<<level<<endl;

	//string filename = argv[1];
	//filename = filename + ".dig.png";
	//cvSaveImage(filename.c_str(), digImg);
	cvReleaseImage(&image);
	cvReleaseImage(&digImg);
	return 0;
}
