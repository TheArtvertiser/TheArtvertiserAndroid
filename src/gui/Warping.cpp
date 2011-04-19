/*
 * Warping.cpp
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#include "Warping.h"
#include <float.h>

#include "ofGraphics.h"

namespace gui{
Warping::Warping(float minDistance)
:quad(4)
,minDistance(minDistance)
,selectedPoint(-1)
{

}

void Warping::enableEvents(){
	ofRegisterMouseEvents(this);
}

void Warping::disableEvents(){
	ofUnregisterMouseEvents(this);
}

void Warping::update(){

}

void Warping::draw(){
	ofPushStyle();
	for(int i=0; i<4; ++i){
		if(selectedPoint==i) ofSetColor(255,0,0);
		else ofSetColor(0,255,0);

		ofCircle(quad[i],4);

		ofSetColor(0,255,0);
		if(i>0) ofLine(quad[i-1],quad[i]);
	}
	ofLine(quad[3],quad[0]);
	ofPopStyle();
}

void Warping::mousePressed(ofMouseEventArgs & mouse){
	float minDistance=FLT_MAX;
	int nearestPoint = -1;
	for(int i=0; i<4; ++i){
		if(quad[i].distance(ofPoint(mouse.x,mouse.y))<minDistance){
			minDistance = quad[i].distance(ofPoint(mouse.x,mouse.y));
			nearestPoint = i;
		}
	}

	if(minDistance<this->minDistance || this->minDistance==0){
		selectedPoint = nearestPoint;
	}
}

void Warping::mouseDragged(ofMouseEventArgs & mouse){
	if(selectedPoint!=-1) quad[selectedPoint].set(mouse.x,mouse.y);
}

void Warping::mouseReleased(ofMouseEventArgs & mouse){
	selectedPoint=-1;
}

const vector<ofPoint> & Warping::getQuad(){
	return quad;
}

void Warping::setMinDistance(float _minDistance){
	minDistance = _minDistance;
}

void Warping::setInitialQuad(const vector<ofPoint> & _quad, const ofPoint & offset){
	if(_quad.size()<4){
		ofLog(OF_LOG_ERROR,"gui::Warping::setInitialQuad: error quad has less than 4 points");
		return;
	}

	for(int i=0; i<4; ++i){
		quad[i] = _quad[i] + offset;
	}

}
}
