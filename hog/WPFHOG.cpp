#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/objdetect/objdetect.hpp>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cmath>
#include "feature.h"

//#define MATLAB
//#define DEBUG

#ifdef MATLAB
#include "mex.h"
#endif

using namespace cv;
using namespace std;

#ifdef MATLAB
/******************************************************
 * Matlab function
 *
 * descriptor = getHoG(I, blockSize, blockStride, cellSize, nbins)
 * - cellSize default [8 8]
 * - blockSize default [32 32]
 *
 ******************************************************/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs == 0)
	{
		mexPrintf("Usage: descriptor = getHoG(I, blockSize, blockStride, cellSize, nbins)\n");
		return;
	}

	if(nrhs > 5)
	{
		mexErrMsgTxt("Invalid input parameter number\n");
	}
	if(nlhs > 1)
	{
		mexErrMsgTxt("Invalid output parameter number\n");
	}

	const mwSize * imsz = mxGetDimensions(prhs[0]);
	int ndim = mxGetNumberOfDimensions(prhs[0]);
	//if(ndim >= 3 && imsz[2] > 1)
	//{
	//	mexErrMsgTxt("Color Image is not supported");
	//}
	if(!mxIsUint8(prhs[0]))
	{
		mexErrMsgTxt("Error: Not uchar image data!");
	}

	unsigned char * inimg = (unsigned char*) mxGetData(prhs[0]);

	int M = imsz[0]; // nrows == height
	int N = imsz[1]; // ncols == width

	int height = M;
	int width = N;
	int npixels = height * width;

	Size blockSize(32, 32);
	if(nrhs >= 2)
	{
		double *tmpmat = (double*) mxGetData(prhs[1]);
		blockSize.width = (int) tmpmat[0];
		blockSize.height = (int) tmpmat[1];
#ifdef DEBUG
		cout<<"blockSize = "<<blockSize.width<<"x"<<blockSize.height<<endl;
#endif
	}

	Size blockStride(8, 8);
	if(nrhs >= 3)
	{
		double *tmpmat = (double*) mxGetData(prhs[2]);
		blockStride.width = (int) tmpmat[0];
		blockStride.height = (int) tmpmat[1];
#ifdef DEBUG
		cout<<"blockStride = "<<blockStride.width<<"x"<<blockStride.height<<endl;
#endif
	}

	Size cellSize(8, 8);
	if(nrhs >= 4)
	{
		double * tmpmat = (double*) mxGetData(prhs[3]);
		cellSize.width = (int) tmpmat[0];
		cellSize.height = (int) tmpmat[1];
		//mexPrintf("cellSize = %dx%d\n", cellSize[0], cellSize[1]);
#ifdef DEBUG
		cout<<"cellSize = "<<cellSize.width<<"x"<<cellSize.height<<endl;
#endif
	}

	int nbins = 9;
	if(nrhs >= 5)
	{
		nbins = mxGetScalar(prhs[4]);
#ifdef DEBUG
		cout<<"nbins = "<<nbins<<endl;
#endif
	}

	Size winSize(width, height);

	IplImage * iplimg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, ndim);
	for(int j = 0; j < height; j++)
	{
		uchar* ptr = (uchar*)(iplimg->imageData + j*iplimg->widthStep);
		for(int i = 0; i < width; i++)
		{
			for(int c = 0; c < ndim; c++)
			{
				ptr[ndim*i+c] = inimg[(ndim-c-1)*npixels + i*height+j];
			}
		}
	}

	FeatureMap ** map = new (FeatureMap*);
	getFeature(iplimg, 8, map);
	normalizehog(*map, 0.2);
	PCAFeature(*map);

	int nFeat = (*map)->numFeatures * (*map)->sizeX * (*map)->sizeY;
	float * _map = (*map)->map;
	vector<float> descriptors(_map, _map+nFeat);

	if(nlhs == 1)
	{
		mxArray * arr = mxCreateDoubleMatrix(1, descriptors.size(), mxREAL);
		double * arrdata = (double*)mxGetData(arr);
		for(int i = 0; i < descriptors.size(); i++)
			arrdata[i] = descriptors[i];
		plhs[0] = arr;
	}

    cvReleaseImage(&iplimg);
	
	delete map;
	delete [] (*map);
	return;
}
#else

int main(int argc, char** argv)
{
	IplImage * image = cvLoadImage(argv[1], 1);

	FeatureMap ** map = new (FeatureMap*);
	getFeature(image, 8, map);
	normalizehog(*map, 0.2);
	PCAFeature(*map);

	int nFeat = (*map)->numFeatures * (*map)->sizeX * (*map)->sizeY;
	float * _map = (*map)->map;
	vector<float> descriptors(_map, _map+nFeat);

	for(int j = 0; j < nFeat; j++)
	{
		printf("%f,", descriptors[j]); 
	}
	printf("\n");
	delete map;
	delete [] (*map);
	return 0;
}
#endif
