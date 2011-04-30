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
#include "Widget.h"
#include "ofRectangle.h"
#include "ofPoint.h"
#include "ofColor.h"

namespace gui{

class CircularPB: public Widget {
public:
	CircularPB( float radius=1 );

	void setRadius(float radius);

	void update();
	void draw();
	void draw(float x, float y);

	void setPosition(const ofPoint & pos);
	void setRect(const ofRectangle & rect);
	void setColor(const ofColor & color);
	ofRectangle getRect();
	float getAspectRatio();

	void enableEvents(){};
	void disableEvents(){};

private:
	float radius;
	ofPolyline bigcircle;
	vector<int> alphas;
	int currentPos;

	ofPoint position;
	ofRectangle rect;

	ofColor color;
};

}

#endif /* CIRCULARPB_H_ */
