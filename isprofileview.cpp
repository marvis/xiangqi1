#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include "path.h"
using namespace std;
using namespace cv;
IplImage * tmpl_profileImage = 0;

void init()
{
	tmpl_profileImage = cvLoadImage(XQTMPLPATH"view_profile.png", 1);
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
	IplImage * profileImage = cropImage(image, 245, 343, 39, 116);
	assert(profileImage->width == tmpl_profileImage->width && profileImage->height == tmpl_profileImage->height);
	double sumdiff = 0.0;
	int width = profileImage->width;
	int height = profileImage->height;
	int nchannels = profileImage->nChannels;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int val1 = CV_IMAGE_ELEM(profileImage, unsigned char, j, i*nchannels + c);
				int val2 = CV_IMAGE_ELEM(tmpl_profileImage, unsigned char, j, i*nchannels + c);
				sumdiff += fabs(val1-val2);
			}
		}
	}
	double avgdiff = sumdiff/(width*height);
	//cerr<<"profileview avgdiff = "<<avgdiff<<endl;
	if(avgdiff < 5) cout<<"true"<<endl;
	else cout<<"false"<<endl;
	cvReleaseImage(&image);
	cvReleaseImage(&profileImage);
}
