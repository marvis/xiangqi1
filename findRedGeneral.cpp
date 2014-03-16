#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

#define DOUBLE_INF 1.79e+308

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
	cvSetImageROI(src, cvRect(x, y, width , height));
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvCopy(src, dst, 0);
    cvResetImageROI(src);
	return dst;
}
double bestMatchingScore(IplImage * smallImg, IplImage * bigImg)
{
	if(!smallImg || !bigImg) return DOUBLE_INF;
	int swid = smallImg->width;
	int shei = smallImg->height;
	int bwid = bigImg->width;
	int bhei = bigImg->height;
	if(swid > bwid || shei > bhei)
	{
		cout<<"smallImg should be smaller than bigImg"<<endl;
		return DOUBLE_INF;
	}
	if(smallImg->nChannels != bigImg->nChannels)
	{
		cout<<"smallImg and bigImg have different channels"<<endl;
		return DOUBLE_INF;
	}
	int nchannels = smallImg->nChannels;
	int offx = (bwid - swid)/2;
	int offy = (bhei - shei)/2;
	double min_sum = -1;
	for(int dy = -3; dy <= 3; dy++)
	{
		for(int dx = -2; dx <= 2; dx++)
		{
			double sum = 0;
			for(int sy = 0; sy < shei; sy++)
			{
				for(int sx = 0; sx < swid; sx++)
				{
					int by = sy + offy + dy;
					int bx = sx + offx + dx;
					if(bx < 0 || bx >= bwid) continue;
					if(by < 0 || by >= bhei) continue;

					for(int c = 0; c < nchannels; c++)
					{
						int sind = sx * nchannels + c + sy * smallImg->widthStep;
						int bind = bx * nchannels + c + by * bigImg->widthStep;
						int diff = (int)(bigImg->imageData[bind]) - (int)(smallImg->imageData[sind]);
						if(diff < 0 ) diff = -diff;
						sum += diff;
					}
				}
			}
			if(min_sum == -1 || sum < min_sum) min_sum = sum;
		}
	}
	return min_sum;
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
	image = cropImage(image, 4, 149, 531, 666);

	IplImage * generalImg = cvLoadImage("/Users/xiaohang/Test/xiangqi/red_general.png", 1);
	if(!generalImg)
	{
		printf("can't find red_general.png\n");
		return -1;
	}

	double x_step = 59;
	double y_step = 65.5;
	int x0 = 29 + 3 * x_step;
	int y0 = 30;

	int positions[18][2] = {
		{x0, y0}, {x0+x_step, y0}, {x0 + 2*x_step, y0},
		{x0, (int)(y0+y_step)}, {x0+x_step, (int)(y0+y_step)}, {x0 + 2*x_step, (int)(y0+y_step)},
		{x0, (int)(y0+2*y_step)}, {x0+x_step, (int)(y0+2*y_step)}, {x0 + 2*x_step, (int)(y0+2*y_step)},

		{x0, (int)(y0+7*y_step)}, {x0+x_step, (int)(y0+7*y_step)}, {x0 + 2*x_step, (int)(y0+7*y_step)},
		{x0, (int)(y0+8*y_step)}, {x0+x_step, (int)(y0+8*y_step)}, {x0 + 2*x_step, (int)(y0+8*y_step)},
		{x0, (int)(y0+9*y_step)}, {x0+x_step, (int)(y0+9*y_step)}, {x0 + 2*x_step, (int)(y0+9*y_step)}
	};

	double min_score = -1;
	int min_id = -1;
	int w0 = 29;
	int x, y;
	for(int i = 0; i < 18; i++)
	{
		x = positions[i][0];
		y = positions[i][1];
		IplImage * chessImg = cropImage(image, x - w0, y - w0, 2*w0+1, 2*w0+1);
		double score = bestMatchingScore(generalImg, chessImg);
		if(min_score == -1 || score < min_score)
		{
			min_score = score;
			min_id = i;
		}
	}
	if(min_id < 9) cout<<"red is up"<<endl;
	else cout<<"red is in bottom"<<endl;
	x = positions[min_id][0];
	y = positions[min_id][1];
	cvCircle(image, cvPoint(x,y), 19, cvScalar(255));

	cvNamedWindow("Orig Image",CV_WINDOW_AUTOSIZE );
	cvShowImage("Orig Image",image );
	waitKey(0);
	cvReleaseImage(&image);

	return 0;

}
