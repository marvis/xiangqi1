#include "opencv/cv.h"   //cv.h OpenCV的主要功能头文件，务必要；
#include "opencv/highgui.h" //显示图像用的，因为用到了显示图片，所以需要包含进去；
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

/*---------------------------------------------------------------------------
   Function :   kth_smallest()
   In       :   array of elements, # of elements in the array, rank k
   Out      :   one element
   Job      :   find the kth smallest element in the array
   Notice   :   use the median() macro defined below to get the median. 

                Reference:

                  Author: Wirth, Niklaus 
                   Title: Algorithms + data structures = programs 
               Publisher: Englewood Cliffs: Prentice-Hall, 1976 
    Physical description: 366 p. 
                  Series: Prentice-Hall Series in Automatic Computation 

 ---------------------------------------------------------------------------*/
// http://ndevilla.free.fr/median/median/index.html


template<class T> void ELEM_SWAP(T &a, T &b) 
{ 
	register T t=a;
	a=b;
	b=t; 
}

template<class T> T kth_smallest(T * a, int n, int k)
{
	long i,j,l,m ;
	T x ;

	l=0 ; m=n-1 ;
	while (l<m) {
		x=a[k] ;
		i=l ;
		j=m ;
		do {
			while (a[i]<x) i++ ;
			while (x<a[j]) j-- ;
			if (i<=j) {
				ELEM_SWAP(a[i],a[j]) ;
				i++ ; j-- ;
			}
		} while (i<=j) ;
		if (j<k) l=i ;
		if (k<i) m=j ;
	}
	return a[k] ;
}

template<class T> T kth_smallest(vector<T> a, int k)
{
	long i,j,l,m ;
	T x ;

	long n = a.size();
	l=0 ; m=n-1 ;
	while (l<m) {
		x=a[k] ;
		i=l ;
		j=m ;
		do {
			while (a[i]<x) i++ ;
			while (x<a[j]) j-- ;
			if (i<=j) {
				ELEM_SWAP(a[i],a[j]) ;
				i++ ; j-- ;
			}
		} while (i<=j) ;
		if (j<k) l=i ;
		if (k<i) m=j ;
	}
	return a[k] ;
}
template<class T> T median(T * a, int n) 
{
	assert(a);
	return kth_smallest(a,n,(((n)&1)?((n)/2):(((n)/2)-1)));
}

template<class T> T median(vector<T> &a) 
{
	assert(!a.empty());
	int n = a.size();
	return kth_smallest(a,(((n)&1)?((n)/2):(((n)/2)-1)));
}
int main(int argc, char ** argv)
{
	if(argc <= 3)
	{
		printf("At least 3 images required!\n");
		return -1;
	}
	IplImage ** allimgs = new IplImage*[argc-1];
	int width = 0, height = 0, nchannels = 0;
	for(int i = 1; i < argc; i++)
	{
		IplImage * image = cvLoadImage(argv[i], 1);
		if(!image)
		{
			printf( "No image data \n" );
			return -1;
		}
		if(width == 0 && height == 0 && nchannels == 0)
		{
			width = image->width;
			height = image->height;
			nchannels = image->nChannels;
		}
		else
		{
			if(width != image->width || height != image->height || nchannels != image->nChannels)
			{
				printf("one image is of different size\n");
				return -1;
			}
		}
		allimgs[i-1] = image;
	}

	IplImage * medianImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, nchannels);
	int widthStep = medianImg->widthStep;
	cout<<"width = "<<width<<" height = "<<height<<" widthStep = "<<widthStep<<" nchannels = "<<nchannels<<endl;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < nchannels; c++)
			{
				int ind = i*nchannels + c + j*widthStep;
				cout<<ind<<" ("<<j<<","<<i<<")"<<endl;
				vector<int> vals;
				for(int n = 0; n < argc-1; n++)
				{
					IplImage * img = allimgs[n];
					int val = img->imageData[ind];
					vals.push_back(val);
				}
				medianImg->imageData[ind] = median(vals);
			}
		}
	}

	cvSaveImage("median_image.png", medianImg);
	cvReleaseImage(&medianImg);
	return 0;

}
