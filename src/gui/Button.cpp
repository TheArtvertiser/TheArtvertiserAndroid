/*
 * Button.cpp
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#include "Button.h"
#include "ofGraphics.h"

namespace gui{
Button::Button(const ofRectangle & rect)
:rect(rect)
,state(Enabled)
{

}

void Button::enableEvents(){
	ofRegisterMouseEvents(this);
}

void Button::disableEvents(){
	ofUnregisterMouseEvents(this);
}

void Button::update(){

}

void Button::draw(){
	draw(rect);
}

void Button::draw(float x, float y){
	draw(rect+ofPoint(x,y));
}

void Button::draw(ofRectangle & rect){
	ofPushStyle();
	ofSetColor(255,255,255);
	ofNoFill();
	switch(state){
	case Focused:
		if(focusedIcon.bAllocated()){
			focusedIcon.draw(rect);
		}else if(icon.bAllocated()){
			icon.draw(rect);
		}else{
			ofRect(rect);
		}
		break;
	case Pressed:
		if(pressedIcon.bAllocated()){
			pressedIcon.draw(rect);
		}else if(icon.bAllocated()){
			icon.draw(rect);
		}else{
			ofRect(rect);
		}
		break;
	default:
		if(icon.bAllocated()){
			icon.draw(rect);
		}else{
			ofRect(rect);
		}
		break;

	}
	ofPopStyle();
}

void Button::mousePressed(ofMouseEventArgs & mouse){
	if(rect.inside(ofPoint(mouse.x,mouse.y))){
		updateState(CursorPressed);
	}
}

void Button::mouseDragged(ofMouseEventArgs & mouse){
	if(rect.inside(ofPoint(mouse.x,mouse.y))){
		updateState(CursorIn);
	}else{
		updateState(CursorOut);
	}
}

void Button::mouseReleased(ofMouseEventArgs & mouse){
	updateState(CursorReleased);
}

void Button::mouseMoved(ofMouseEventArgs & mouse){
	if(rect.inside(ofPoint(mouse.x,mouse.y))){
		updateState(CursorIn);
	}else{
		updateState(CursorOut);
	}
}

void Button::updateState(Transition transition){
	//cout << "Button::updateState from state " << state << " by transition " << transition << " to state ";
	switch(transition){
	case CursorPressed:
		if(state==Enabled || state==Focused){
			state=Pressed;
		}
		break;
	case CursorReleased:
		if(state==Pressed){
			state=Enabled;
			bool pressed = true;
			ofNotifyEvent(pressedE,pressed,this);
		}else if(state==OutPressed){
			state=Enabled;
		}
		break;
	case CursorIn:
		if(state==Enabled){
			state=Focused;
		}
		break;
	case CursorOut:
		if(state==Focused){
			state=Enabled;
		}else if(state==Pressed){
			state=OutPressed;
		}
	}
	//cout << state << endl;
}

void Button::setRect(const ofRectangle & _rect){
	rect = _rect;
}

ofRectangle Button::getRect(){
	return rect;
}

float Button::getAspectRatio(){
	if(icon.bAllocated()){
		return icon.getWidth()/icon.getHeight();
	}else{
		return 1;
	}
}

void Button::setIcon(ofImage & _icon){
	icon = _icon;
	rect.width = icon.getWidth();
	rect.height = icon.getHeight();
}

void Button::setFocusedIcon(ofImage & icon){
	focusedIcon = icon;
}

void Button::setPressedIcon(ofImage & icon){
	pressedIcon = icon;
}

void Button::setPosition(const ofPoint & pos){
	rect.x = pos.x;
	rect.y = pos.y;
}

Button::State Button::getState(){
	return state;
}

}
