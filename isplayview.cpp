#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>

using namespace std;
using namespace cv;
IplImage * tmpl_playImage = 0;

void init()
{
	tmpl_playImage = cvLoadImage("/Users/xiaohang/Test/xiangqi/templates/view_play.png", 1);
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
	IplImage * playImage = cropImage(image, 26, 84, 64, 79);
	assert(playImage->width == tmpl_playImage->width && playImage->height == tmpl_playImage->height);
	double sumdiff = 0.0;
	int width = playImage->width;
	int height = playImage->height;
	int nchannels = playImage->nChannels;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int val1 = CV_IMAGE_ELEM(playImage, unsigned char, j, i*nchannels + c);
				int val2 = CV_IMAGE_ELEM(tmpl_playImage, unsigned char, j, i*nchannels + c);
				sumdiff += fabs(val1-val2);
			}
		}
	}
	double avgdiff = sumdiff/(width*height);
	//cerr<<"playview: avgdiff = "<<avgdiff<<endl;
	if(avgdiff < 5) cout<<"true"<<endl;
	else cout<<"false"<<endl;
	cvReleaseImage(&image);
	cvReleaseImage(&playImage);
}
