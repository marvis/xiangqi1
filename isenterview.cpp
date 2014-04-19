#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include "path.h"
using namespace std;
using namespace cv;
IplImage * tmpl_enterImage1 = 0;
IplImage * tmpl_enterImage2 = 0;

void init()
{
	tmpl_enterImage1 = cvLoadImage(XQTMPLPATH"view_enter1.png", 1);
	tmpl_enterImage2 = cvLoadImage(XQTMPLPATH"view_enter2.png", 1);
}

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
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
		return 0;
	}
	init();
	IplImage * image = cvLoadImage(argv[1], 1);
	IplImage * enterImage1 = cropImage(image, 204, 445, 299, 58);
	assert(enterImage1->width == tmpl_enterImage1->width && enterImage1->height == tmpl_enterImage1->height);
	double sumdiff1 = 0.0;
	int width1 = enterImage1->width;
	int height1 = enterImage1->height;
	int nchannels = enterImage1->nChannels;
	for(int j = 0; j < height1; j++)
	{
		for(int i = 0; i < width1; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int val1 = CV_IMAGE_ELEM(enterImage1, unsigned char, j, i*nchannels + c);
				int val2 = CV_IMAGE_ELEM(tmpl_enterImage1, unsigned char, j, i*nchannels + c);
				sumdiff1 += fabs(val1-val2);
			}
		}
	}
	double avgdiff1 = sumdiff1/(width1*height1);
	//cerr<<"enterview avgdiff = "<<avgdiff<<endl;
	if(avgdiff1 < 5)
	{
		cout<<"true"<<endl;
		cvReleaseImage(&image);
		cvReleaseImage(&enterImage1);
		cvReleaseImage(&tmpl_enterImage1);
		return 0;
	}

	IplImage * enterImage2 = cropImage(image, 122, 701, 128, 37);
	assert(enterImage2->width == tmpl_enterImage2->width && enterImage2->height == tmpl_enterImage2->height);
	double sumdiff2 = 0.0;
	int width2 = enterImage2->width;
	int height2 = enterImage2->height;
	nchannels = enterImage2->nChannels;
	for(int j = 0; j < height2; j++)
	{
		for(int i = 0; i < width2; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int val1 = CV_IMAGE_ELEM(enterImage2, unsigned char, j, i*nchannels + c);
				int val2 = CV_IMAGE_ELEM(tmpl_enterImage2, unsigned char, j, i*nchannels + c);
				sumdiff2 += fabs(val1-val2);
			}
		}
	}
	double avgdiff2 = sumdiff2/(width2*height2);
	//cerr<<"enterview avgdiff = "<<avgdiff<<endl;
	if(avgdiff2 < 5)
	{
		cout<<"true"<<endl;
	}
	else cout<<"false"<<endl;
	cvReleaseImage(&image);
	cvReleaseImage(&enterImage1);
	cvReleaseImage(&enterImage2);
	cvReleaseImage(&tmpl_enterImage1);
	cvReleaseImage(&tmpl_enterImage2);
	return 0;
}
