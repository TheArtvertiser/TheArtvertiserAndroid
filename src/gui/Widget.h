/*
 * Widget.h
 *
 *  Created on: 20/04/2011
 *      Author: arturo
 */

#ifndef WIDGET_H_
#define WIDGET_H_

#include "ofPoint.h"
#include "ofRectangle.h"

namespace gui{
	class Widget {
	public:
		virtual ~Widget(){  }

		virtual void setPosition(const ofPoint & pos)=0;
		virtual void setRect(const ofRectangle & rect)=0;
		virtual ofRectangle getRect()=0;
		virtual float getAspectRatio()=0;

		virtual void update()=0;
		virtual void draw()=0;

		virtual void enableEvents()=0;
		virtual void disableEvents()=0;
	};
}
#endif /* WIDGET_H_ */
