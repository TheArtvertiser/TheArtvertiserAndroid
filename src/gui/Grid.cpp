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
{
	ofAddListener(ofEvents.windowResized,this,&Grid::windowResized);
}

Grid::~Grid(){
	ofRemoveListener(ofEvents.windowResized,this,&Grid::windowResized);
}

void Grid::setPosition(const ofPoint & _pos){
	pos = _pos;
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
	ofPoint nextPos(pos);
	for(int i=0;i<(int)widgets.size();i++){
		Widget * widget = widgets[i].get();
		float ratio = widget->getAspectRatio();
		float height = cellWidth/ratio;
		widget->setRect(ofRectangle(nextPos.x,nextPos.y,cellWidth,height));

		if(nextPos.x + (cellWidth + hSpacing)*2 > ofGetWidth() - hSpacing){
			nextPos.x = pos.x;
			nextPos.y += cellHeight + vSpacing;
		}else{
			nextPos.x += cellWidth + hSpacing;
		}
	}
}

void Grid::windowResized(ofResizeEventArgs & window){
	repositionWidgets();
}

float Grid::getCellWidth(){
	return cellWidth;
}

float Grid::getCellHeight(){
	return cellHeight;
}
}
