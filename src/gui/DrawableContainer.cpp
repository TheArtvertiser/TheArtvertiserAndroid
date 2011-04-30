/*
 * DrawableContainer.cpp
 *
 *  Created on: 29/04/2011
 *      Author: arturo
 */

#include "DrawableContainer.h"

namespace gui{

DrawableContainer::DrawableContainer() {

}

DrawableContainer::DrawableContainer(ofPtr<ofBaseDraws> & drawable){
	setDrawable(drawable);
}

DrawableContainer::~DrawableContainer() {
}

void DrawableContainer::setDrawable(ofPtr<ofBaseDraws> & _drawable){
	drawable = _drawable;
	if(rectangle.width==0)rectangle.width = drawable->getWidth();
	if(rectangle.height==0)rectangle.height = drawable->getHeight();
}

ofPtr<ofBaseDraws> & DrawableContainer::getDrawable(){
	return drawable;
}

void DrawableContainer::setPosition(const ofPoint & pos){
	rectangle.x = pos.x;
	rectangle.y = pos.y;
}

void DrawableContainer::setRect(const ofRectangle & rect){
	rectangle = rect;
}

void DrawableContainer::setWidth(float width){
	rectangle.height = width/getAspectRatio();
	rectangle.width = width;
}

void DrawableContainer::setHeight(float height){
	rectangle.width = height*getAspectRatio();
	rectangle.height = height;
}

ofRectangle DrawableContainer::getRect(){
	return rectangle;
}

float DrawableContainer::getAspectRatio(){
	if(rectangle.height==0) return 0;
	return rectangle.width / rectangle.height;
}

void DrawableContainer::update(){

}

void DrawableContainer::draw(){
	drawable->draw(rectangle);
}

void DrawableContainer::enableEvents(){

}

void DrawableContainer::disableEvents(){

}

}
