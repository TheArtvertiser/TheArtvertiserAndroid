/*
 * OnlineArtverts.cpp
 *
 *  Created on: 01/05/2011
 *      Author: arturo
 */

#include "OnlineArtverts.h"
#include "ofAppRunner.h"
#include "ofTypes.h"
#include "Button.h"
#include "ofGraphics.h"

OnlineArtverts::OnlineArtverts(string url)
:state(Countries)
,circularPB(30)
,loading(false)
{
	list.setPosition(ofPoint(0,20));
	list.setLeftMargin(20);
	list.setWidth(ofGetWidth()-40);
	list.setElementHeight(30);
	setURL(url);
	ofRegisterURLNotification(this);
	circularPB.setColor(ofColor(190,190,190));
	circularPB.setPosition(ofPoint(ofGetWidth()*0.5,ofGetHeight()*0.5));
	ofAddListener(ofEvents.windowResized,this,&OnlineArtverts::windowResized);
}

OnlineArtverts::~OnlineArtverts() {
	// TODO Auto-generated destructor stub
}

void OnlineArtverts::setURL(string _url){
	url = _url;
}

void OnlineArtverts::start(){
	listCountries();
}

void OnlineArtverts::stop(){

}

void OnlineArtverts::update(){
	list.update();
	if(loading){
		circularPB.update();
	}
}

void OnlineArtverts::draw(){
	ofSetColor(255,255,255);
	list.draw();
	if(loading){
		circularPB.draw();
	}
}

void OnlineArtverts::windowResized(ofResizeEventArgs & window){
	list.setWidth(window.width-40);
	circularPB.setPosition(ofPoint(window.width*0.5,window.height*0.5));
}

bool OnlineArtverts::back(){
	switch(state){
	case Countries:
		return false;
	case Cities:
		listCountries();
		return true;
	case Roads:
		listCities(currentCountry);
		return true;
	case Artverts:
		listRoads(currentCountry,currentCity);
		return true;
	}
}

void OnlineArtverts::urlResponse(ofHttpResponse & response){
	ofLogVerbose("OnlineArtverts","new response: " + response.request.name);
	if(response.request.name=="listcountries"){
		list.clear();
		string country;
		while(!response.data.isLastLine()){
			country=response.data.getNextLine();
			if(country!=""){
				ofLogVerbose("OnlineArtverts",country);
				ofPtr<gui::Button> button = ofPtr<gui::Button>(new gui::Button(country,ofColor(255,255,255,255)));
				list.addWidget(button);
				ofAddListener(button->pressedE,this,&OnlineArtverts::countrySelected);
			}
		}
		list.enableEvents();
		loading = false;
	}
	if(response.request.name=="listcities"){
		list.clear();
		string city;
		while(!response.data.isLastLine()){
			city=response.data.getNextLine();
			if(city!=""){
				ofLogVerbose("OnlineArtverts",city);
				ofPtr<gui::Button> button = ofPtr<gui::Button>(new gui::Button(city,ofColor(255,255,255,255)));
				list.addWidget(button);
				ofAddListener(button->pressedE,this,&OnlineArtverts::citySelected);
			}
		}
		list.enableEvents();
		loading = false;
	}
	if(response.request.name=="listroads"){
		list.clear();
		string road;
		while(!response.data.isLastLine()){
			road=response.data.getNextLine();
			if(road!=""){
				ofLogVerbose("OnlineArtverts",road);
				ofPtr<gui::Button> button = ofPtr<gui::Button>(new gui::Button(road,ofColor(255,255,255,255)));
				list.addWidget(button);
				ofAddListener(button->pressedE,this,&OnlineArtverts::roadSelected);
			}
		}
		list.enableEvents();
		loading = false;
	}
}

void OnlineArtverts::listCountries(){
	state = Countries;
	list.disableEvents();
	loading = true;
	ofLoadURLAsync(url+"/listcountries.of","listcountries");
}

void OnlineArtverts::listCities(const string & country){
	state = Cities;
	list.disableEvents();
	loading = true;
	ofLoadURLAsync(url+"/listcities.of?country=" + country,"listcities");
}

void OnlineArtverts::listRoads(const string & country, const string & city){
	state = Roads;
	list.disableEvents();
	loading = true;
	ofLoadURLAsync(url+"/listroads.of?country=" +country+ "&city="+city ,"listroads");
}

void OnlineArtverts::countrySelected(const void * sender, bool & pressed){
	gui::Button* button = (gui::Button*)sender;
	currentCountry = button->getText();
	listCities(currentCountry);
}

void OnlineArtverts::citySelected(const void * sender, bool & pressed){
	gui::Button* button = (gui::Button*)sender;
	currentCity = button->getText();
	listRoads(currentCountry,currentCity);
}

void OnlineArtverts::roadSelected(const void * sender, bool & pressed){
	/*state = Roads;
	gui::Button* button = (gui::Button*)sender;
	currentCity = button->getText();
	listRoads(currentCountry,currentCity);*/
}
