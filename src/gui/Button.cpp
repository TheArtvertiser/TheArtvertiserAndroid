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
	ofPushStyle();
	ofSetColor(255,255,255);
	if(icon.bAllocated()){
		icon.draw(rect);
	}else{
		ofRect(rect);
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
		updateState(CursorIn);
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

void Button::setIcon(ofImage & _icon){
	icon = _icon;
}

Button::State Button::getState(){
	return state;
}

}
