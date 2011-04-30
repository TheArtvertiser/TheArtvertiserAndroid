/*
 * Label.cpp
 *
 *  Created on: 29/04/2011
 *      Author: arturo
 */

#include "Label.h"
#include "ofGraphics.h"

namespace gui{

Label::Label() {
	// TODO Auto-generated constructor stub

}

Label::~Label() {
	// TODO Auto-generated destructor stub
}

void Label::setPosition(const ofPoint & pos){
	position = pos;
}

void Label::setRect(const ofRectangle & rect){
	position.x = rect.x;
	position.y = rect.y;
}

ofRectangle Label::getRect(){
	return ofRectangle(position.x,position.y,text.size()*10,10);
}

float Label::getAspectRatio(){
	return getRect().width/getRect().height;
}

void Label::update(){

}

void Label::draw(){
	ofPushStyle();
	ofSetColor(0,0,0);
	ofDrawBitmapString(text,position.x,position.y+10);
	ofPopStyle();
}

void Label::enableEvents(){

}

void Label::disableEvents(){

}

void Label::setText(const string & _text){
	text = _text;
}

}
