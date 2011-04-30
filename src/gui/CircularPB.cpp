/*
 * CircularPB.cpp
 *
 *  Created on: 19/04/2011
 *      Author: arturo
 */

#include "CircularPB.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofMath.h"

namespace gui{
CircularPB::CircularPB( float radius )
:radius(radius)
,alphas(radius+1)
,currentPos(0)
,color(40,40,40)
{
	bigcircle.arc(0,0,0,radius,radius,0,360,radius);
}

void CircularPB::setRadius(float _radius){
	radius = _radius;
	rect.width = rect.height = radius*2;
	bigcircle.clear();
	bigcircle.arc(0,0,0,radius,radius,0,360,radius);
	alphas.resize(bigcircle.size());
}

void CircularPB::update(){
	currentPos = int(ofGetElapsedTimeMillis()*10*0.001)%bigcircle.size();

	for(int i=0;i<(int)bigcircle.size();i++){
		float currentAngle = 360.f/float(bigcircle.size())*float(currentPos);
		float iAngle = 360.f/float(bigcircle.size())*float(i);
		float diff = abs(ofAngleDifferenceDegrees(iAngle,currentAngle));
		//if(diff<0) diff=255;

		alphas[i] = 255-diff*(255.f/(360.f/3.f));
	}
}

void CircularPB::draw(){
	ofPushStyle();
	for(int i=0;i<(int)bigcircle.size();i++){
		ofSetColor(color,alphas[i]);
		ofPoint pos = bigcircle[i] + position;
		ofCircle(pos,3+radius/25.f);
	}
	ofPopStyle();
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
	radius = rect.width*0.5;
	bigcircle.clear();
	bigcircle.arc(0,0,0,rect.width*0.5,rect.height*0.5,0,360,radius);
	alphas.resize(bigcircle.size());
}


void CircularPB::setColor(const ofColor & _color){
	color = _color;
}

ofRectangle CircularPB::getRect(){
	return rect;
}

float CircularPB::getAspectRatio(){
	return rect.width / rect.height;
}

}
