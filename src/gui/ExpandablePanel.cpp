/*
 * ExpandablePanel.cpp
 *
 *  Created on: 29/04/2011
 *      Author: arturo
 */

#include "ExpandablePanel.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

namespace gui{

ExpandablePanel::ExpandablePanel()
:isExpanded(false)
{
	// TODO Auto-generated constructor stub

}

ExpandablePanel::~ExpandablePanel() {
	// TODO Auto-generated destructor stub
}

void ExpandablePanel::setRect(const ofRectangle & _rect){
	rect = _rect;
	VFrame::setWidth(rect.width);
	VFrame::setPosition(ofPoint(rect.x,rect.y));
}

void ExpandablePanel::setRectCompressed(const ofRectangle & rect){
	rectCompressed = rect;
	if(!isExpanded){
		setRect(rect);
	}
}

void ExpandablePanel::setRectExpanded(const ofRectangle & rect){
	rectExpanded = rect;
	if(isExpanded){
		setRect(rect);
	}
}

void ExpandablePanel::expand(){
	setRect(rectExpanded);
	isExpanded = true;
}

void ExpandablePanel::compress(){
	setRect(rectCompressed);
	isExpanded = false;
}

void ExpandablePanel::toggleExpand(){
	isExpanded = !isExpanded;
	setRect(isExpanded?rectExpanded:rectCompressed);
}


void ExpandablePanel::update(){
	VFrame::update();
}

void ExpandablePanel::draw(){
	glEnable(GL_SCISSOR_TEST);
	glScissor(rect.x,ofGetHeight()-(rect.y+rect.height),rect.width,rect.height);
	ofRect(rect);
	VFrame::draw();
	glDisable(GL_SCISSOR_TEST);
}


void ExpandablePanel::mousePressed(ofMouseEventArgs & mouse){

}

void ExpandablePanel::mouseDragged(ofMouseEventArgs & mouse){

}

void ExpandablePanel::mouseReleased(ofMouseEventArgs & mouse){
	if(rectCompressed.inside(ofPoint(mouse.x,mouse.y))){
		toggleExpand();
	}
}

void ExpandablePanel::mouseMoved(ofMouseEventArgs & mouse){

}

void ExpandablePanel::enableEvents(){
	ofRegisterMouseEvents(this);
	Container::enableEvents();
}

void ExpandablePanel::disableEvents(){
	ofUnregisterMouseEvents(this);
	Container::disableEvents();
}

}
