#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include "path.h"

using namespace std;
using namespace cv;
IplImage * tmpl_selectImage = 0;

void init()
{
	tmpl_selectImage = cvLoadImage(XQTMPLPATH"view_select.png", 1);
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
	IplImage * selectImage = cropImage(image, 84, 322, 296, 235);
	assert(selectImage->width == tmpl_selectImage->width && selectImage->height == tmpl_selectImage->height);
	double sumdiff = 0.0;
	int width = selectImage->width;
	int height = selectImage->height;
	int nchannels = selectImage->nChannels;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int val1 = CV_IMAGE_ELEM(selectImage, unsigned char, j, i*nchannels + c);
				int val2 = CV_IMAGE_ELEM(tmpl_selectImage, unsigned char, j, i*nchannels + c);
				sumdiff += fabs(val1-val2);
			}
		}
	}
	double avgdiff = sumdiff/(width*height);
	//cerr<<"selectview: avgdiff = "<<avgdiff<<endl;
	if(avgdiff < 5) cout<<"true"<<endl;
	else cout<<"false"<<endl;
	cvReleaseImage(&image);
	cvReleaseImage(&selectImage);
}
