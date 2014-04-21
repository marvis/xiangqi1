#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include "path.h"
using namespace std;
using namespace cv;
#define VIEWNUM 12

enum {STARTVIEW = 0, SELECTVIEW, LOGINVIEW, BACKVIEW, CONFIRMVIEW, NETERRORVIEW, ENTERVIEW, PREPAREVIEW, CHANGINGTABLEVIEW, GAMEOVERVIEW, PLAYVIEW, FLAPPYBIRDVIEW};

string view_paths[VIEWNUM];
CvRect rects[VIEWNUM];

string viewnames[] = {"startview", "selectview", "loginview", "backview", "confirmview", "neterrorview", "enterview", "prepareview", "changingtableview", "gameoverview", "playview", "flappybirdview"};

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
	cvSetImageROI(src, cvRect(x, y, width , height));
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvCopy(src, dst, 0);
    cvResetImageROI(src);
	return dst;
}
void init()
{
	int id = 0;
	view_paths[id] = XQTMPLPATH"view_start.png"; 
	rects[id++] = cvRect(380, 674, 306, 99);

	view_paths[id] = XQTMPLPATH"view_select.png"; 
	rects[id++] = cvRect(84, 322, 296, 235);

	view_paths[id] = XQTMPLPATH"view_login.png"; 
	rects[id++] = cvRect(72, 363, 180, 204);
	
	view_paths[id] = XQTMPLPATH"view_back.png"; 
	rects[id++] = cvRect(310, 728, 95, 94);
	
	view_paths[id] = XQTMPLPATH"view_confirm.png"; 
	rects[id++] = cvRect(308, 724, 99, 97);

	view_paths[id] = XQTMPLPATH"view_neterror.png"; 
	rects[id++] = cvRect(308, 730, 96, 90);

	view_paths[id] = XQTMPLPATH"view_enter.png"; 
	rects[id++] = cvRect(63, 564, 42, 95);

	view_paths[id] = XQTMPLPATH"view_prepare.png"; 
	rects[id++] = cvRect(288, 803, 150, 89);

	view_paths[id] = XQTMPLPATH"view_changingtable.png"; 
	rects[id++] = cvRect(371, 613, 166, 47);

	view_paths[id] = XQTMPLPATH"view_gameover.png"; 
	rects[id++] = cvRect(54, 924, 39, 69);

	view_paths[id] = XQTMPLPATH"view_play.png"; 
	rects[id++] = cvRect(26, 84, 64, 79);

	view_paths[id] = XQTMPLPATH"view_flappybird.png"; 
	rects[id++] = cvRect(305, 380, 55, 90);
}

bool isView(IplImage * image, int id)
{
	if(id < 0 || id >= VIEWNUM) return false;
	CvRect rect = rects[id];

	IplImage * myImg = cropImage(image, rect.x, rect.y, rect.width, rect.height);
	IplImage * tmplImg = cvLoadImage(view_paths[id].c_str(), 1);
	assert(myImg->width == tmplImg->width && myImg->height == tmplImg->height && myImg->nChannels == tmplImg->nChannels);

	double sumdiff = 0.0;
	int width = myImg->width;
	int height = myImg->height;
	int nchannels = myImg->nChannels;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int val1 = CV_IMAGE_ELEM(myImg, unsigned char, j, i*nchannels + c);
				int val2 = CV_IMAGE_ELEM(tmplImg, unsigned char, j, i*nchannels + c);
				sumdiff += fabs(val1-val2);
			}
		}
	}
	double avgdiff = sumdiff/(width*height);
	cvReleaseImage(&myImg);
	cvReleaseImage(&tmplImg);
	if(avgdiff < 5) return true;
	else return false;
}

string whichView(IplImage * image)
{
	for(int i = 0; i < VIEWNUM; i++)
	{
		if(isView(image, i)) return viewnames[i];
		//cout<<"not "<<viewnames[i]<<endl;
	}
	return "unknowview";
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
	string viewname = whichView(image);
	cout<<viewname<<endl;
	cvReleaseImage(&image);
	return 0;
}
