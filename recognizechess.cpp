#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;
#define DOUBLE_INF 1.79e+308
enum ChessType {BLACK_CHE = 0, BLACK_MA, BLACK_XIANG, BLACK_SHI, BLACK_JIANG, BLACK_ZU,
	RED_CHE, RED_MA, RED_XIANG, RED_SHI, RED_JIANG, RED_ZU, LAST_POS, UNKNOWNCHESS};

string chessnames[16] = {
	"BC", "BM", "BX", "BS", "BJ", "BP","BZ", 
	"RC", "RM", "RX", "RS", "RJ", "RP","RZ", 
	"LP", "NA"
};

IplImage ** templChesses = 0;

bool initTemplChesses()
{
	templChesses = new IplImage * [15];
	int id = 0;
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_black_che.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_black_ma.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_black_xiang.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_black_shi.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_black_jiang.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_black_pao.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_black_zu.png", 1);

	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_red_che.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_red_ma.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_red_xiang.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_red_shi.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_red_jiang.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_red_pao.png", 1);
	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_red_zu.png", 1);

	templChesses[id++] = cvLoadImage("/Users/xiaohang/Test/xiangqi/xiangqi_blank.png", 1);

	bool isok = true;
	for(int i = 0; i < 15; i++)
	{
		if(templChesses[i] == 0)
		{
			cout<<"unable to load "<<chessnames[i]<<endl;
			isok = false;
		}
	}
	if(!isok) return false;

	int width = templChesses[0]->width;
	int height = templChesses[0]->height;
	int hlfwid = width/2;
	int hlfhei = height/2;
	int nchannels = templChesses[0]->nChannels;
	for(int i = 0; i < 15; i++)
	{
		if(templChesses[i]->width != width ||
				templChesses[i]->height != height ||
				templChesses[i]->nChannels != nchannels)
		{
			cout<<"template images have different size"<<endl;
			return false;
		}
		//cvSmooth(templChesses[i], templChesses[i], CV_GAUSSIAN, 3,3,0,0);
		IplImage * hlfImg = cvCreateImage(cvSize(hlfwid, hlfhei), IPL_DEPTH_8U, nchannels);
		cvResize(templChesses[i], hlfImg);
		cvReleaseImage(&templChesses[i]);
		templChesses[i] = hlfImg;
	}
	return isok;
}

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
	cvSetImageROI(src, cvRect(x, y, width , height));
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvCopy(src, dst, 0);
    cvResetImageROI(src);
	return dst;
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

ChessType whichChess(IplImage * image)
{
	int width = image->width; 
	int height = image->height;
	double min_score = -1;
	int min_id = -1;
	for(int i = 0; i < 15; i++)
	{
		//IplImage * diffImg = imageDiff(image, templChesses[i]);
		//double score = sumImageRoi(diffImg, 0, 0, width, height);
		double score = bestMatchingScore(image, templChesses[i]);
		if(min_score == -1)
		{
			min_score = score;
			min_id = i;
		}
		else if(score < min_score)
		{
			min_score = score;
			min_id = i;
		}
	}
	return (ChessType) min_id;
}
int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		printf("No image data \n");
		return -1;
	}
	if(!initTemplChesses())
	{
		printf("unable to load template chess images");
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

	IplImage * image2 = cropImage(image, 4, 149, 531, 660);
	cvReleaseImage(&image);
	image = image2;

	IplImage * bkgImg2 = cropImage(bkgImg, 4, 149, 531, 660);
	cvReleaseImage(&bkgImg);
	bkgImg = bkgImg2;

	IplImage * diffImg = imageDiff(bkgImg, image);
	
	IplImage * grayImg = cvCreateImage(cvSize(diffImg->width, diffImg->height), IPL_DEPTH_8U, 1);
	cvCvtColor(diffImg,grayImg,CV_BGR2GRAY);

	// draw start
	int x0 = 29;
	int y0 = 29;
	int x_step = 59;
	int y_step = 66;
	int id = 1;

	//the font variable    
	CvFont font;    
	double hScale=1;   
	double vScale=1;    
	int lineWidth=2;// 相当于写字的线条    
	// 初始化字体   
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//初始化字体，准备写到图片上的   

	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			int w0 = 29;
			int x = x0 + i * x_step;
			int y = y0 + j * y_step;
			double sumval = sumImageRoi(grayImg, x-w0, y-w0, 2*w0+1, 2*w0+1);
			if(sumval > 50000)
			{
				int w1 = 19;
				IplImage * chessImg = cropImage(image, x-w1, y-w1, 2*w1+1, 2*w1+1);
				//cvSmooth(chessImg, chessImg, CV_GAUSSIAN,3,3,0,0);
				IplImage * hlfChessImg = cvCreateImage(cvSize(w1, w1), IPL_DEPTH_8U, chessImg->nChannels);
				cvResize(chessImg, hlfChessImg);
				cvReleaseImage(&chessImg);
				chessImg = hlfChessImg;

				ChessType type = whichChess(chessImg);
				cout<<chessnames[type]<<" ";

				string showMsg = chessnames[type];
				showMsg = showMsg.substr(1,1);
				if(type == 14) showMsg = "E";
				// cvPoint 为起笔的x，y坐标   
				if(type < 7)
					cvPutText(image,showMsg.c_str(),cvPoint(x-w0,y),&font,CV_RGB(0,0,255));//在图片中输出字符
				else
					cvPutText(image,showMsg.c_str(),cvPoint(x-w0,y),&font,CV_RGB(255,0,0));//在图片中输出字符

			}
		}
		cout<<endl;
	}
	// draw end
	
	cvNamedWindow("Orig Image",CV_WINDOW_AUTOSIZE );
	cvShowImage("Orig Image",image );
	waitKey(0);

	cvReleaseImage(&image);
	cvReleaseImage(&bkgImg);
	cvReleaseImage(&grayImg);

	for(int i = 0; i < 15; i++) cvReleaseImage(&templChesses[i]);
	delete [] templChesses; templChesses = 0;
	return 0;
}
