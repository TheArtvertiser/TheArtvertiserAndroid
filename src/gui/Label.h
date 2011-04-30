/*
 * Label.h
 *
 *  Created on: 29/04/2011
 *      Author: arturo
 */

#ifndef LABEL_H_
#define LABEL_H_

#include "ofRectangle.h"
#include "ofPoint.h"

#include "Widget.h"


namespace gui{

class Label: public Widget {
public:
	Label();
	~Label();

	void setPosition(const ofPoint & pos);
	void setRect(const ofRectangle & rect);
	ofRectangle getRect();
	float getAspectRatio();

	void update();
	void draw();

	void enableEvents();
	void disableEvents();

	void setText(const string & text);

private:
	ofPoint position;
	string text;
};

}
#endif /* LABEL_H_ */
