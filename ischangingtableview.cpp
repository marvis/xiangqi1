#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include "path.h"

using namespace std;
using namespace cv;
IplImage * tmpl_changingtableImage = 0;

void init()
{
	tmpl_changingtableImage = cvLoadImage(XQTMPLPATH("view_changingtable.png"), 1);
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
	IplImage * changingtableImage = cropImage(image, 371, 613, 166, 47);
	assert(changingtableImage->width == tmpl_changingtableImage->width && changingtableImage->height == tmpl_changingtableImage->height);
	double sumdiff = 0.0;
	int width = changingtableImage->width;
	int height = changingtableImage->height;
	int nchannels = changingtableImage->nChannels;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int val1 = CV_IMAGE_ELEM(changingtableImage, unsigned char, j, i*nchannels + c);
				int val2 = CV_IMAGE_ELEM(tmpl_changingtableImage, unsigned char, j, i*nchannels + c);
				sumdiff += fabs(val1-val2);
			}
		}
	}
	double avgdiff = sumdiff/(width*height);
	//cerr<<"changingtableview avgdiff = "<<avgdiff<<endl;
	if(avgdiff < 5) cout<<"true"<<endl;
	else cout<<"false"<<endl;
	cvReleaseImage(&image);
	cvReleaseImage(&changingtableImage);
	return 0;
}
