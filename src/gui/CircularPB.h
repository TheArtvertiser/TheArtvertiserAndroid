/*
 * CircularPB.h
 *
 *  Created on: 19/04/2011
 *      Author: arturo
 */

#ifndef CIRCULARPB_H_
#define CIRCULARPB_H_

#include "ofPolyline.h"
#include <vector>

namespace gui{

class CircularPB {
public:
	CircularPB( float radius=1 );

	void setRadius(float radius);

	void update();
	void draw();
	void draw(float x, float y);

private:
	float radius;
	ofPolyline bigcircle;
	vector<int> alphas;
	int currentPos;
};

}

#endif /* CIRCULARPB_H_ */
