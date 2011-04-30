/*
 * Grid.cpp
 *
 *  Created on: 21/04/2011
 *      Author: arturo
 */

#include "Grid.h"
#include "ofAppRunner.h"

namespace gui{
Grid::Grid()
:cellWidth(0)
,cellHeight(0)
,hSpacing(0)
,vSpacing(0)
,wholescreen(true)
{
	ofAddListener(ofEvents.windowResized,this,&Grid::windowResized);
}

Grid::~Grid(){
	ofRemoveListener(ofEvents.windowResized,this,&Grid::windowResized);
}

void Grid::setPosition(const ofPoint & _pos){
	rect.x = _pos.x;
	rect.y = _pos.y;
	rect.width = ofGetWidth()-rect.x;
	rect.height = ofGetHeight()-rect.y;
	wholescreen = true;
	repositionWidgets();
}

void Grid::setRectangle(const ofRectangle & _rect){
	rect = _rect;
	wholescreen = false;
	repositionWidgets();
}

void Grid::setCellSize(float _cellW, float _cellH){
	cellWidth = _cellW;
	cellHeight = _cellH;
	repositionWidgets();

}
void Grid::setSpacing(float _hSpacing, float _vSpacing){
	hSpacing = _hSpacing;
	vSpacing = _vSpacing;
	repositionWidgets();
}

void Grid::repositionWidgets(){
	ofPoint nextPos(rect.x,rect.y);
	for(int i=0;i<(int)widgets.size();i++){
		Widget * widget = widgets[i].get();

		float ratio = widget->getAspectRatio();
		float width,height;
		if(ratio>1){
			width = cellWidth;
			height = width/ratio;
		}else{
			height = cellHeight;
			width = height * ratio;
		}

		widget->setRect(ofRectangle(nextPos.x,nextPos.y,width,height));
		if(nextPos.x + (cellWidth + hSpacing)*2 > rect.x + rect.width - hSpacing){
			nextPos.x = rect.x;
			nextPos.y += cellHeight + vSpacing;
		}else{
			nextPos.x += cellWidth + hSpacing;
		}
	}
}

void Grid::windowResized(ofResizeEventArgs & window){
	if(wholescreen){
		rect.width = ofGetWidth()-rect.x;
		rect.height = ofGetHeight()-rect.y;
	}
	repositionWidgets();
}

float Grid::getCellWidth(){
	return cellWidth;
}

float Grid::getCellHeight(){
	return cellHeight;
}
}
