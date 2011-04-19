/*
  Copyright 2007, 2008 Computer Vision Lab,
  Ecole Polytechnique Federale de Lausanne (EPFL), Switzerland.
  All rights reserved.

  Authors: Julien Pilet (http://cvlab.epfl.ch/~jpilet)

  This file is part of the ferns_demo software.

  ferns_demo is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  ferns_demo is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ferns_demo; if not, write to the Free Software Foundation, Inc., 51 Franklin
  Street, Fifth Floor, Boston, MA 02110-1301, USA
*/
/*
 * Julien Pilet, may 2008
 */
#include <assert.h>

/* computes the homography sending [0,0] , [0,1], [1,1] and [1,0]
 * to x,y,z and w.
 */
void homography_from_4pt(const float *x, const float *y, const float *z, const float *w, float cgret[8])
{
	float t1 = x[0];
	float t2 = z[0];
	float t4 = y[1];
	float t5 = t1 * t2 * t4;
	float t6 = w[1];
	float t7 = t1 * t6;
	float t8 = t2 * t7;
	float t9 = z[1];
	float t10 = t1 * t9;
	float t11 = y[0];
	float t14 = x[1];
	float t15 = w[0];
	float t16 = t14 * t15;
	float t18 = t16 * t11;
	float t20 = t15 * t11 * t9;
	float t21 = t15 * t4;
	float t24 = t15 * t9;
	float t25 = t2 * t4;
	float t26 = t6 * t2;
	float t27 = t6 * t11;
	float t28 = t9 * t11;
	float t30 = 0.1e1 / (-t24 + t21 - t25 + t26 - t27 + t28);
	float t32 = t1 * t15;
	float t35 = t14 * t11;
	float t41 = t4 * t1;
	float t42 = t6 * t41;
	float t43 = t14 * t2;
	float t46 = t16 * t9;
	float t48 = t14 * t9 * t11;
	float t51 = t4 * t6 * t2;
	float t55 = t6 * t14;
	cgret[0] = -(-t5 + t8 + t10 * t11 - t11 * t7 - t16 * t2 + t18 - t20 + t21 * t2) * t30;
	cgret[1] = (t5 - t8 - t32 * t4 + t32 * t9 + t18 - t2 * t35 + t27 * t2 - t20) * t30;
	cgret[2] = t1;
	cgret[3] = (-t9 * t7 + t42 + t43 * t4 - t16 * t4 + t46 - t48 + t27 * t9 - t51) * t30;
	cgret[4] = (-t42 + t41 * t9 - t55 * t2 + t46 - t48 + t55 * t11 + t51 - t21 * t9) * t30;
	cgret[5] = t14;
	cgret[6] = (-t10 + t41 + t43 - t35 + t24 - t21 - t26 + t27) * t30;
	cgret[7] = (-t7 + t10 + t16 - t43 + t27 - t28 - t21 + t25) * t30;
	//cgret[8] = 1;
	

}


void homography_transform(const float a[2], const float H[3][3], float r[2])
{
	float z = H[2][0]*a[0] + H[2][1]*a[1] + H[2][2];
	r[0] = (H[0][0]*a[0] + H[0][1]*a[1] + H[0][2])/z;
	r[1] = (H[1][0]*a[0] + H[1][1]*a[1] + H[1][2])/z;
}

#ifdef DEBUG_CMPHOMO
static bool eps_cmp2(const float a[2], const float b[2])
{
	float eps = 1e-3;
	float dx = a[0]-b[0], dy =a[1]-b[1];

	return (dx*dx <eps && dy*dy<eps);
}
#endif

void homography_from_4corresp(
		const float *a, const float *b, const float *c, const float *d,
		const float *x, const float *y, const float *z, const float *w, float R[3][3])
{
	float Hr[3][3], Hl[3][3];

	homography_from_4pt(a,b,c,d,&Hr[0][0]);
	homography_from_4pt(x,y,z,w,&Hl[0][0]);

	// the following code computes R = Hl * inverse Hr
	float t2 = Hr[1][1]-Hr[2][1]*Hr[1][2];
	float t4 = Hr[0][0]*Hr[1][1];
	float t5 = Hr[0][0]*Hr[1][2];
	float t7 = Hr[1][0]*Hr[0][1];
	float t8 = Hr[0][2]*Hr[1][0];
	float t10 = Hr[0][1]*Hr[2][0];
	float t12 = Hr[0][2]*Hr[2][0];
	float t15 = 1/(t4-t5*Hr[2][1]-t7+t8*Hr[2][1]+t10*Hr[1][2]-t12*Hr[1][1]);
	float t18 = -Hr[1][0]+Hr[1][2]*Hr[2][0];
	float t23 = -Hr[1][0]*Hr[2][1]+Hr[1][1]*Hr[2][0];
	float t28 = -Hr[0][1]+Hr[0][2]*Hr[2][1];
	float t31 = Hr[0][0]-t12;
	float t35 = Hr[0][0]*Hr[2][1]-t10;
	float t41 = -Hr[0][1]*Hr[1][2]+Hr[0][2]*Hr[1][1];
	float t44 = t5-t8;
	float t47 = t4-t7;
	float t48 = t2*t15;
	float t49 = t28*t15;
	float t50 = t41*t15;
	R[0][0] = Hl[0][0]*t48+Hl[0][1]*(t18*t15)-Hl[0][2]*(t23*t15);
	R[0][1] = Hl[0][0]*t49+Hl[0][1]*(t31*t15)-Hl[0][2]*(t35*t15);
	R[0][2] = -Hl[0][0]*t50-Hl[0][1]*(t44*t15)+Hl[0][2]*(t47*t15);
	R[1][0] = Hl[1][0]*t48+Hl[1][1]*(t18*t15)-Hl[1][2]*(t23*t15);
	R[1][1] = Hl[1][0]*t49+Hl[1][1]*(t31*t15)-Hl[1][2]*(t35*t15);
	R[1][2] = -Hl[1][0]*t50-Hl[1][1]*(t44*t15)+Hl[1][2]*(t47*t15);
	R[2][0] = Hl[2][0]*t48+Hl[2][1]*(t18*t15)-t23*t15;
	R[2][1] = Hl[2][0]*t49+Hl[2][1]*(t31*t15)-t35*t15;
	R[2][2] = -Hl[2][0]*t50-Hl[2][1]*(t44*t15)+t47*t15;

#ifdef DEBUG_CMPHOMO
	// sanity check
	float uv[2];
	homography_transform(a, R, uv);
	assert(eps_cmp2(uv,x));

	homography_transform(b, R, uv);
	assert(eps_cmp2(uv,y));

	homography_transform(c, R, uv);
	assert(eps_cmp2(uv,z));

	homography_transform(d, R, uv);
	assert(eps_cmp2(uv,w));
#endif
}
