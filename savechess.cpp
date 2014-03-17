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
	for(int dy = -2; dy <= 2; dy++)
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

	IplImage * bkgImg = cvLoadImage("/Users/xiaohang/Test/xiangqi/background.png", 1);
	if(!bkgImg)
	{
		printf( "No background.png in current folder");
		return -1;
	}

	IplImage * image2 = cropImage(image, 4, 149, 534, 666);
	cvReleaseImage(&image);
	image = image2;

	IplImage * bkgImg2 = cropImage(bkgImg, 4, 149, 534, 666);
	cvReleaseImage(&bkgImg);
	bkgImg = bkgImg2;

	IplImage * diffImg = imageDiff(bkgImg, image);
	
	IplImage * grayImg = cvCreateImage(cvSize(diffImg->width, diffImg->height), IPL_DEPTH_8U, 1);
	cvCvtColor(diffImg,grayImg,CV_BGR2GRAY);

	// draw start
	double x_step = 59;
	double y_step = 65.5;
	
	int x0 = 29 + 3 * x_step;
	int y0 = 30;
	// find red general start

	IplImage * generalImg = cvLoadImage("/Users/xiaohang/Test/xiangqi/red_general.png", 1);
	if(!generalImg)
	{
		printf("can't find red_general.png\n");
		return -1;
	}

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
	bool red_is_bot = true;
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
	if(min_id < 9) red_is_bot = false;
	else red_is_bot = true;

	// find red general stop
	if(red_is_bot)
	{
		x0 = 29;
		y0 = 30;
	}
	else
	{
		x0 = 31;
		y0 = 33;
	}
	int id = 1;
	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			int w0 = 29;
			int x = (int)(x0 + i * x_step);
			int y = (int)(y0 + j * y_step);
			double sumval = sumImageRoi(grayImg, x - w0, y - w0, 2*w0+1, 2*w0+1);
			cout<<sumval<<" ";
			if(sumval > 50000)
			{
				IplImage * chessImg = cropImage(image, x - w0, y - w0, 2*w0+1, 2*w0+1);
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
