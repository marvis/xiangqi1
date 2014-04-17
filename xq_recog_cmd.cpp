#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <sstream>
#include "path.h"

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

char fennames[16] = {
	'r', 'n', 'b', 'a', 'k', 'c', 'p',
	'R', 'N', 'B', 'A', 'K', 'C', 'P',
	' ', ' '};

IplImage ** templChesses = 0;
IplImage * blackfromChess = 0;
IplImage * blacktoChess = 0;
IplImage * redfromChess = 0;
IplImage * redtoChess = 0;
IplImage * masktoChess = 0;

string matrix2fenstr(char matrix[10][9])
{
	ostringstream oss;
	char c;
	for(int j = 0; j < 10; j++)
	{
		int count = 0;
		for(int i = 0; i < 9; i++)
		{
			if(matrix[j][i] == ' ') count++;
			else
			{
				if(count > 0)
				{
					oss<<count;
					count = 0;
				}
				oss<<matrix[j][i];
			}
		}
		if(count>0)
		{
			oss<<count;
		}
		if(j!=9) oss<<'/';
	}
	return oss.str();
}

bool initTemplChesses()
{
	templChesses = new IplImage * [15];
	int id = 0;
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_black_che.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_black_ma.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_black_xiang.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_black_shi.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_black_jiang.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_black_pao.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_black_zu.png"), 1);

	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_red_che.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_red_ma.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_red_xiang.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_red_shi.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_red_jiang.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_red_pao.png"), 1);
	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_red_zu.png"), 1);

	templChesses[id++] = cvLoadImage(XQTMPLPATH("xiangqi_blank.png"), 1);

	blackfromChess = cvLoadImage(XQTMPLPATH("black_move_from_hlf.png"), 1);
	redfromChess = cvLoadImage(XQTMPLPATH("red_move_from_hlf.png"), 1);

	blacktoChess = cvLoadImage(XQTMPLPATH("black_move_to_hlf.png"), 1);
	redtoChess = cvLoadImage(XQTMPLPATH("red_move_to_hlf.png"), 1);
	masktoChess = cvLoadImage(XQTMPLPATH("mask_move_to_hlf.png"), 1);

	if(blackfromChess->width != redfromChess->width || blackfromChess->height != redfromChess->height)
		cerr<<"black_move_from.png and red_move_from.png have different size"<<endl;
	if(blacktoChess->width != redtoChess->width || blacktoChess->height != redtoChess->height)
		cerr<<"black_move_to.png and red_move_to.png have different size"<<endl;
	if(blacktoChess->width != masktoChess->width || blacktoChess->height != masktoChess->height)
		cerr<<"black_move_to.png and mask_move_to.png have different size"<<endl;

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
			cerr<<"template images have different size"<<endl;
			return false;
		}
		//cvSmooth(templChesses[i], templChesses[i], CV_GAUSSIAN, 3,3,0,0);
		// 缩小图像大小, 达到平滑的作用, 同时加快速度
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

IplImage * cropImage3(IplImage * src, int x, int y, int width, int height)
{
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvSetZero(dst);
	int nchannels = src->nChannels;
	for(int j = 0; j < height; j++)
	{
		int jj = j + y;
		for(int i = 0; i < width; i++)
		{
			int ii = i + x;
			if(jj < 0 || jj >= src->height || ii < 0 || ii >= src->width) continue;
			for(int c = 0; c < src->nChannels; c++)
			{
				CV_IMAGE_ELEM(dst, unsigned char, j, nchannels*i+c) = CV_IMAGE_ELEM(src, unsigned char, jj, nchannels*ii + c);
			}
		}
	}
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
						int bval = CV_IMAGE_ELEM(bigImg, unsigned char, by, bx*nchannels + c);
						int sval = CV_IMAGE_ELEM(smallImg, unsigned char, sy, sx*nchannels + c);
						int diff = bval - sval;
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

double min_movefrom_score(IplImage * image)
{
	if(image->width != blackfromChess->width+4 || image->height != blackfromChess->height+4)
	{
		cerr<<"unable to check movestatus_from because of invalid size"<<endl;
		return 0;
	}
	int nchannels = image->nChannels;
	double min_score = DOUBLE_INF;
	for(int dy = 1; dy < 3; dy++)
	{
		for(int dx = 1; dx < 3; dx++)
		{
			double black_diff = 0.0;
			double red_diff = 0.0;
			for(int j = 0; j < blackfromChess->height; j++)
			{
				for(int i = 0; i < blackfromChess->width; i++)
				{
					for(int c = 0; c < nchannels; c++)
					{
						double val0 = CV_IMAGE_ELEM(image, unsigned char, j+dy, (i+dx)*nchannels + c);
						double val1 = CV_IMAGE_ELEM(blackfromChess, unsigned char, j, i*nchannels + c);
						double val2 = CV_IMAGE_ELEM(redfromChess, unsigned char, j, i*nchannels + c);
						black_diff += fabs(val0-val1);
						red_diff += fabs(val0-val2);
					}
				}
			}
			if(black_diff < min_score) min_score = black_diff;
			if(red_diff < min_score) min_score = red_diff;
		}
	}
	return min_score;
}

double min_moveto_score(IplImage * image)
{
	if(image->width != blacktoChess->width+4 || image->height != blacktoChess->height+4)
	{
		cerr<<"unable to check movestatus_to because of invalid size"<<endl;
		return 0;
	}
	double min_score = DOUBLE_INF;
	int nchannels = image->nChannels;
	for(int dy = 1; dy < 3; dy++)
	{
		for(int dx = 1; dx < 3; dx++)
		{
			double black_diff = 0.0;
			double red_diff = 0.0;
			for(int j = 0; j < blacktoChess->height; j++)
			{
				for(int i = 0; i < blacktoChess->width; i++)
				{
					int maskval = CV_IMAGE_ELEM(masktoChess, unsigned char, j, i);
					if(maskval > 128)
					{
						for(int c = 0; c < nchannels; c++)
						{
							double val0 = CV_IMAGE_ELEM(image, unsigned char, j+dy, (i+dx)*nchannels + c);
							double val1 = CV_IMAGE_ELEM(blacktoChess, unsigned char, j, i*nchannels + c);
							double val2 = CV_IMAGE_ELEM(redtoChess, unsigned char, j, i*nchannels + c);
							black_diff += fabs(val0-val1);
							red_diff += fabs(val0-val2);
						}
					}
				}
			}
			if(black_diff < min_score) min_score = black_diff;
			if(red_diff < min_score) min_score = red_diff;
		}
	}
	return min_score;
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
	if(argc < 2)
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

	IplImage * bkgImg = cvLoadImage(XQTMPLPATH("background.png"), 1);
	if(!bkgImg)
	{
		printf( "No background.png in current folder");
		return -1;
	}

	if(image->width != 540 || image->height != 960)
	{
		IplImage * image1 = cvCreateImage(cvSize(540, 960), IPL_DEPTH_8U, 3);
		cvResize(image, image1);
		cvReleaseImage(&image);
		image = image1;
	}

	IplImage * image2 = cropImage(image, 0, 139, 540, 686);
	cvReleaseImage(&image);
	image = image2;

	IplImage * bkgImg2 = cropImage(bkgImg, 0, 139, 540, 686);
	cvReleaseImage(&bkgImg);
	bkgImg = bkgImg2;

	IplImage * diffImg = imageDiff(bkgImg, image);
	
	IplImage * grayImg = cvCreateImage(cvSize(diffImg->width, diffImg->height), IPL_DEPTH_8U, 1);
	cvCvtColor(diffImg,grayImg,CV_BGR2GRAY);

	// draw start
	double x_step = 59;
	double y_step = 65.5;

	int x0 = 33 + 3 * x_step;
	int y0 = 40;
	
	// find red general start

	IplImage * generalImg = cvLoadImage(XQTMPLPATH("red_general.png"), 1);
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
	if(min_id < 9) 
	{
		red_is_bot = false;
		//cout<<"red is up"<<endl;
	}
	else 
	{
		//cout<<"red is bot"<<endl;
		red_is_bot = true;
	}

	// find red general stop
	if(red_is_bot)
	{
		x0 = 33;
		y0 = 40;
	}
	else
	{
		x0 = 35;
		y0 = 43;
	}
	int id = 1;

	//the font variable    
	CvFont font;    
	double hScale=1;   
	double vScale=1;    
	int lineWidth=2;// 相当于写字的线条    
	// 初始化字体   
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);//初始化字体，准备写到图片上的   

	char chessmatrix[10][9];
	string chessmatrix_names[10][9];
	IplImage * drawImage = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, image->nChannels);
	cvCopy(image, drawImage);
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
				cvReleaseImage(&chessImg);
				chessmatrix[j][i] = fennames[type];
				chessmatrix_names[j][i] = chessnames[type];

				string showMsg = chessnames[type];
				showMsg = showMsg.substr(1,1);
				if(type == 14) showMsg = "E";
				// cvPoint 为起笔的x，y坐标   
				if(type < 7)
					cvPutText(drawImage,showMsg.c_str(),cvPoint(x-w0,y),&font,CV_RGB(0,0,255));//在图片中输出字符
				else
					cvPutText(drawImage,showMsg.c_str(),cvPoint(x-w0,y),&font,CV_RGB(255,0,0));//在图片中输出字符

			}
			else
			{
				chessmatrix[j][i] = ' ';
				//cout<<"   ";
			}
		}
		//cout<<endl;
	}

	double min_score_from = DOUBLE_INF;
	double min_score_to = DOUBLE_INF;
	int fromj=-1, fromi=-1, toj=-1, toi=-1;
	for(int j = 0; j < 10; j++)
	{
		for(int i = 0; i < 9; i++)
		{
			int w0 = 29;
			int x = x0 + i * x_step;
			int y = y0 + j * y_step;
				
			int w2 = 29, h2 = 32;
			IplImage * chessImg_from = cropImage3(image, x - w2, y - h2, 2*w2+1, 2*h2+1);
			IplImage * hlfChessImg_from = cvCreateImage(cvSize(w2, h2), IPL_DEPTH_8U, chessImg_from->nChannels);
			cvResize(chessImg_from, hlfChessImg_from);
			cvReleaseImage(&chessImg_from);
			chessImg_from = hlfChessImg_from;
			double score_from = min_movefrom_score(chessImg_from);
			if(score_from < min_score_from)
			{
				min_score_from = score_from;
				fromj = j;
				fromi = i;
			}

			int w3 = 34, h3 = 37;
			IplImage * chessImg_to = cropImage3(image, x - w3, y - h3, 2*w3+1, 2*h3+1);
			IplImage * hlfChessImg_to = cvCreateImage(cvSize(w3, h3), IPL_DEPTH_8U, chessImg_to->nChannels);
			cvResize(chessImg_to, hlfChessImg_to);
			char to_str[255]; sprintf(to_str, "to_%d_%d.png", j, i);
			chessImg_to = hlfChessImg_to;
			double score_to = min_moveto_score(chessImg_to);
			if(score_to < min_score_to)
			{
				min_score_to = score_to;
				toj = j;
				toi = i;
			}

			cvReleaseImage(&chessImg_from);
			cvReleaseImage(&chessImg_to);
		}
	}
	//cout<<chessmatrix_names[toj][toi]<<" ("<<fromj<<","<<fromi<<") -> ("<<toj<<","<<toi<<")"<<endl;

	if(argc > 2)
	{
		cvSaveImage(argv[2], drawImage);
	}
	string fenstr = matrix2fenstr(chessmatrix);
	cout<<fenstr;
	if(fenstr == "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR") cout<<" w w";
	else if(fenstr == "RNBAKABNR/9/1C5C1/P1P1P1P1P/9/9/p1p1p1p1p/1c5c1/9/rnbakabnr") cout<<" b w";
	else
	{
		if(red_is_bot) cout<<" w";
		else cout<<" b";
		if(chessmatrix[toj][toi] >= 'A' && chessmatrix[toj][toi] <= 'Z') cout<<" b";
		else cout<<" w";
	}
	
	cout<<endl;
	
	cvReleaseImage(&image);
	cvReleaseImage(&drawImage);
	cvReleaseImage(&bkgImg);
	cvReleaseImage(&grayImg);

	for(int i = 0; i < 15; i++) cvReleaseImage(&templChesses[i]);
	delete [] templChesses; templChesses = 0;
	return 0;
}
