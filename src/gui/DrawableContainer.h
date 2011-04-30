/*
 * DrawableContainer.h
 *
 *  Created on: 29/04/2011
 *      Author: arturo
 */

#ifndef DRAWABLECONTAINER_H_
#define DRAWABLECONTAINER_H_

#include "ofBaseTypes.h"
#include "ofTypes.h"
#include "Widget.h"

namespace gui{

class DrawableContainer: public Widget {
public:
	DrawableContainer();
	DrawableContainer(ofPtr<ofBaseDraws> & drawable);
	~DrawableContainer();

	void setDrawable(ofPtr<ofBaseDraws> & drawable);
	ofPtr<ofBaseDraws> & getDrawable();

	void setPosition(const ofPoint & pos);
	void setRect(const ofRectangle & rect);

	void setWidth(float width);
	void setHeight(float height);

	ofRectangle getRect();
	float getAspectRatio();

	void update();
	void draw();

	void enableEvents();
	void disableEvents();

private:
	ofPtr<ofBaseDraws> drawable;
	ofRectangle rectangle;
};

}
#endif /* DRAWABLECONTAINER_H_ */
