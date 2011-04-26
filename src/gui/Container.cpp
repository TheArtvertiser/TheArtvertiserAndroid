/*
 * Container.cpp
 *
 *  Created on: 21/04/2011
 *      Author: arturo
 */

#include "Container.h"

namespace gui{

Container::Container()
:eventsEnabled(false)
{

}

Container::~Container(){
	disableEvents();
}

void Container::addWidget(ofPtr<Widget> widget){
	widgets.push_back(widget);
	repositionWidgets();
	if(eventsEnabled) widget->enableEvents();
}

void Container::removeWidget(ofPtr<Widget> widget){
	for(int i=0; i<(int)widgets.size(); i++){
		if(widgets[i] == widget){
			widget->disableEvents();
			widgets.erase(widgets.begin()+i);
			repositionWidgets();
			return;
		}
	}
}

ofPtr<Widget> Container::getWidget(int i){
	return widgets[i];
}

ofPtr<Widget> Container::operator[](int i){
	return widgets[i];
}

int Container::size(){
	return widgets.size();
}

void Container::clear(){
	for(int i=0;i<(int)widgets.size();i++){
		widgets[i]->disableEvents();
	}
	widgets.clear();
}

void Container::update(){
	for(int i=0; i<(int)widgets.size(); i++){
		widgets[i]->update();
	}
}

void Container::draw(){
	for(int i=0; i<(int)widgets.size(); i++){
		widgets[i]->draw();
	}
}

void Container::enableEvents(){
	for(int i=0; i<(int)widgets.size(); i++){
		widgets[i]->enableEvents();
	}
	eventsEnabled = true;
}

void Container::disableEvents(){
	for(int i=0; i<(int)widgets.size(); i++){
		widgets[i]->disableEvents();
	}
	eventsEnabled = false;
}
}
