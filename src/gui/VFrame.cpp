/*
 * VFrame.cpp
 *
 *  Created on: 20/04/2011
 *      Author: arturo
 */

#include "VFrame.h"

namespace gui{


VFrame::VFrame()
:width(0)
,vSpacing(0)
,leftMargin(0)
{

}

void VFrame::setPosition(const ofPoint & _pos){
	pos = _pos;
	repositionWidgets();
}

void VFrame::setWidth(float _width){
	width = _width;
	repositionWidgets();
}

void VFrame::setVSpacing(float _vSpacing){
	vSpacing = _vSpacing;
	repositionWidgets();
}


void VFrame::setLeftMargin(float margin){
	leftMargin = margin;
}

void VFrame::repositionWidgets(){
	ofPoint nextPos(pos);
	nextPos.x += leftMargin;
	nextPos.y += vSpacing;
	for(int i=0;i<(int)widgets.size();i++){
		Widget * widget = widgets[i].get();
		float ratio = widget->getAspectRatio();
		float height = width/ratio;
		widget->setRect(ofRectangle(nextPos.x,nextPos.y,width,height));

		nextPos.y += widget->getRect().height + vSpacing;
	}
}

}
