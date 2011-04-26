/*
 * CircularPB.cpp
 *
 *  Created on: 19/04/2011
 *      Author: arturo
 */

#include "CircularPB.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

namespace gui{
CircularPB::CircularPB( float radius )
:radius(radius)
,alphas(15)
,currentPos(0)
{
	bigcircle.arc(0,0,radius,radius,0,360,15);
}

void CircularPB::setRadius(float _radius){
	radius = _radius;
	rect.width = rect.height = radius;
	bigcircle.arc(0,0,radius,radius,0,360,15);
}

void CircularPB::update(){
	currentPos = int(ofGetElapsedTimeMillis()*10*0.001)%15;
	//alphas[currentPos] = 255;
	/*for(int i=currentPos-1;i>=0;i--){
		alphas[i] = MAX(255-(currentPos-i)*(255.f/4.f),0);
	}
	for(int i=currentPos+1;i<15;i++){
		alphas[i] = 0;
	}*/

	for(int i=0;i<15;i++){
		alphas[i] = MAX(255-abs(currentPos-i)*(255.f/4.f),0);
	}
}

void CircularPB::draw(){
	for(int i=0;i<15;i++){
		ofSetColor(190,190,190,alphas[i]);
		ofPoint pos = bigcircle[i] + position;
		ofCircle(pos,3);
	}
}

void CircularPB::draw(float x, float y){
	ofPoint oldPosition = position;
	position += ofPoint(x,y);
	draw();
	position = oldPosition;
}

void CircularPB::setPosition(const ofPoint & pos){
	position = pos;
}

void CircularPB::setRect(const ofRectangle & _rect){
	rect = _rect;
	position.x = rect.x + rect.width*0.5;
	position.y = rect.y + rect.height*0.5;
	bigcircle.arc(0,0,rect.width,rect.height,0,360,15);
}

ofRectangle CircularPB::getRect(){
	return rect;
}

float CircularPB::getAspectRatio(){
	return rect.width / rect.height;
}

}
