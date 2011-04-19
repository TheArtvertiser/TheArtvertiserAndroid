#include <cv.h>
#include <opencv.hpp>
#include <stdio.h>

//Definitions for corner detectors.
//These are internal functions and should not be called directly.
//Input checking is only done in cvCornerFast


CvPoint* icvFast9Detect(const unsigned char* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
int* icvFast9Score(const unsigned char* i, int stride, CvPoint* corners, int num_corners, int b);

CvPoint* icvFast10Detect(const unsigned char* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
int* icvFast10Score(const unsigned char* i, int stride, CvPoint* corners, int num_corners, int b);

CvPoint* icvFast11Detect(const unsigned char* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
int* icvFast11Score(const unsigned char* i, int stride, CvPoint* corners, int num_corners, int b);

CvPoint* icvFast12Detect(const unsigned char* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
int* icvFast12Score(const unsigned char* i, int stride, CvPoint* corners, int num_corners, int b);

/* This defines non-strict maxima */
#define Compare(X, Y) ((X)>=(Y))

/* This is a fast, integer only, sparse nonmaximal suppression. */
/* probably only useful for FAST corner detection */
CvPoint* icvFastNonmaxSuppression(const CvPoint* corners, const int* scores, int num_corners, int* ret_num_nonmax)
{
	int num_nonmax=0;
	int last_row;
	int* row_start;
	int i, j;
	CvPoint* ret_nonmax;
	const int sz = (int)num_corners; 

	/*Point above points (roughly) to the pixel above the one of interest, if there
    is a feature there.*/
	int point_above = 0;
	int point_below = 0;

	
	if(num_corners < 1)
	{
		*ret_num_nonmax = 0;
		return 0;
	}

	ret_nonmax = (CvPoint*)malloc(num_corners * sizeof(CvPoint));

	/* Find where each row begins
	   (the corners are output in raster scan order). A beginning of -1 signifies
	   that there are no corners on that row. */
	last_row = corners[num_corners-1].y;
	row_start = (int*)malloc((last_row+1)*sizeof(int));

	for(i=0; i < last_row+1; i++)
		row_start[i] = -1;
	
	{
		int prev_row = -1;
		for(i=0; i< num_corners; i++)
			if(corners[i].y != prev_row)
			{
				row_start[corners[i].y] = i;
				prev_row = corners[i].y;
			}
	}
	
	
	
	for(i=0; i < sz; i++)
	{
		int score = scores[i];
		CvPoint pos = corners[i];
			
		/*Check left */
		if(i > 0)
			if(corners[i-1].x == pos.x-1 && corners[i-1].y == pos.y && Compare(scores[i-1], score))
				continue;
			
		/*Check right*/
		if(i < (sz - 1))
			if(corners[i+1].x == pos.x+1 && corners[i+1].y == pos.y && Compare(scores[i+1], score))
				continue;
			
		/*Check above (if there is a valid row above)*/
		if(pos.y != 0 && row_start[pos.y - 1] != -1) 
		{
			/*Make sure that current point_above is one
			  row above.*/
			if(corners[point_above].y < pos.y - 1)
				point_above = row_start[pos.y-1];
			
			/*Make point_above point to the first of the pixels above the current point,
			  if it exists.*/
			for(; corners[point_above].y < pos.y && corners[point_above].x < pos.x - 1; point_above++)
			{}
			
			
			for(j=point_above; corners[j].y < pos.y && corners[j].x <= pos.x + 1; j++)
			{
				int x = corners[j].x;
				if( (x == pos.x - 1 || x ==pos.x || x == pos.x+1) && Compare(scores[j], score))
					goto cont;
			}
			
		}
			
		/*Check below (if there is anything below)*/
		if(pos.y != last_row && row_start[pos.y + 1] != -1 && point_below < sz) /*Nothing below*/
		{
			if(corners[point_below].y < pos.y + 1)
				point_below = row_start[pos.y+1];
			
			/* Make point below point to one of the pixels belowthe current point, if it
			   exists.*/
			for(; point_below < sz && corners[point_below].y == pos.y+1 && corners[point_below].x < pos.x - 1; point_below++)
			{}

			for(j=point_below; j < sz && corners[j].y == pos.y+1 && corners[j].x <= pos.x + 1; j++)
			{
				int x = corners[j].x;
				if( (x == pos.x - 1 || x ==pos.x || x == pos.x+1) && Compare(scores[j],score))
					goto cont;
			}
		}
		
		ret_nonmax[num_nonmax++] = corners[i];
		cont:
			;
	}

	free(row_start);
	*ret_num_nonmax = num_nonmax;
	return ret_nonmax;
}


CVAPI(void)
cvCornerFast( const CvArr* input_image, int threshold, int N, int nonmax_suppression, int* ret_number_of_corners, CvPoint** ret_corners)
{
    CV_FUNCNAME( "cvCornerFast" );

	int rows, cols, stride;
	uchar* data;
	
	CvPoint *corners=0, *nonmax_corners=0;
	int* scores=0;
	int num_corners=0, num_nonmax=0;

	CvMat stub, *src;

    __BEGIN__;

	if(!input_image)
        CV_ERROR( CV_StsNullPtr, "" );

	if(!ret_number_of_corners)
        CV_ERROR( CV_StsNullPtr, "" );

	if( N < 9 || N > 12)
        CV_ERROR( CV_StsOutOfRange, "Corner arc length must be 9, 10, 11 or 12." );
	
	
	/* Make sure the input is unsigned char */
	/* In principle, FAST can work on any type, but trees are only included for unsigned char */
    src = (CvMat*)input_image;
    CV_CALL( src = cvGetMat( input_image, &stub ));

    if( CV_MAT_TYPE(src->type) != CV_8UC1 )
        CV_ERROR( CV_StsUnsupportedFormat, "Input must be 8uC1." );

	rows = src->rows;
	cols = src->cols;
	stride = src->step;
	data = src->data.ptr;
	
	//Detect corners
	if(N == 9)
		corners = icvFast9Detect(data, cols, rows, stride, threshold, &num_corners);
	else if(N == 10)
		corners = icvFast10Detect(data, cols, rows, stride, threshold, &num_corners);
	else if(N == 11)
		corners = icvFast11Detect(data, cols, rows, stride, threshold, &num_corners);
	else
		corners = icvFast12Detect(data, cols, rows, stride, threshold, &num_corners);

	//Compute scores
	if(nonmax_suppression)
	{
		if(N == 9)
			scores = icvFast9Score(data, stride, corners, num_corners, threshold);
		else if(N == 10)
			scores = icvFast10Score(data, stride, corners, num_corners, threshold);
		else if(N == 11)
			scores = icvFast11Score(data, stride, corners, num_corners, threshold);
		else
			scores = icvFast12Score(data, stride, corners, num_corners, threshold);
	}

	*ret_number_of_corners = num_corners;
	*ret_corners = corners;
	
	//Do nonmax suppression if need be
	if(nonmax_suppression)
	{
		nonmax_corners = icvFastNonmaxSuppression(corners, scores, num_corners, & num_nonmax);

		*ret_corners = nonmax_corners;
		*ret_number_of_corners = num_nonmax; 

		free(corners);
	}

	free(scores);

    __END__;
}

