#ifndef CVFAST_H
#define CVFAST_H

#include <cvaux.h>
CVAPI(void)  cvCornerFast( const CvArr* image, int threshold, int N,
                           int nonmax_suppression, int* ret_number_of_corners,
						   CvPoint** ret_corners);
#endif
