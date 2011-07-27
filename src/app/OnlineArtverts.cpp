/*
 Copyright 2011 Arturo Castro <arturo@arturocastro.net>,

 Distributed under the terms of the GNU General Public License v3.

 This file is part of The Artvertiser for Android.

 The Artvertiser for Android is free software: you can redistribute
 it and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.

 The Artvertiser for Android is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with The Artvertiser for Android.  If not, see <http://www.gnu.org/licenses/>.
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
,downloader(this)
{
	list.setPosition(ofPoint(0,20));
	list.setLeftMargin(20);
	list.setWidth(ofGetWidth()-40);
	list.setElementHeight(40);
	list.setKeepAspectRatio(false);
	setURL(url);
	ofRegisterURLNotification(this);
	circularPB.setColor(ofColor(190,190,190));
	circularPB.setPosition(ofPoint(ofGetWidth()*0.5,ofGetHeight()*0.5));
	ofAddListener(ofEvents.windowResized,this,&OnlineArtverts::windowResized);

	ofImage cameraIcon;
	cameraIcon.loadImage("icons/camera.png");

	grid.setCellSize(cameraIcon.getWidth()*1.4, cameraIcon.getWidth()*1.4*3./4.);
	grid.setSpacing(20,20);
	grid.setRectangle(ofRectangle(20,20,ofGetWidth(),ofGetHeight()));

	listTTF = "fonts/FreeSans.ttf";
	listTTFSize = 14;
}

OnlineArtverts::~OnlineArtverts() {
	// TODO Auto-generated destructor stub
}

void OnlineArtverts::setURL(string _url){
	url = _url;
}

void OnlineArtverts::setIconCache(ofPtr<gui::IconCache> _iconCache){
	iconCache = _iconCache;
}

void OnlineArtverts::setComm(ofPtr<Comm> _comm){
	this->comm = _comm;
}

void OnlineArtverts::setup(){
	allDownloaded.setFont(iconCache->getFont(listTTF,30));
	allDownloaded.setPosition({30,30});
	allDownloaded.setColor({255,255,255});
}

void OnlineArtverts::start(){
	listCountries();
}

void OnlineArtverts::stop(){

}

void OnlineArtverts::update(){
	if(state==Artverts){
		grid.update();
	}else{
		list.update();
	}
	if(loading){
		circularPB.update();
	}

}

void OnlineArtverts::draw(){
	ofSetColor(255,255,255);
	if(state==Artverts){
		if(grid.empty()) allDownloaded.draw();
		else grid.draw();
	}else{
		list.draw();
	}
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
		listCities(currentCountry);
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
				button->setFont(iconCache->getFont(listTTF,listTTFSize));
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
				button->setFont(iconCache->getFont(listTTF,listTTFSize));
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
				button->setFont(iconCache->getFont(listTTF,listTTFSize));
				list.addWidget(button);
				ofAddListener(button->pressedE,this,&OnlineArtverts::roadSelected);
			}
		}
		list.enableEvents();
		loading = false;
	}
	if(response.request.name=="listartverts"){
		list.clear();
		grid.clear();
		artverts.clear();
		string artvertStr;
		while(!response.data.isLastLine()){
			artvertStr=response.data.getNextLine();
			if(artvertStr!=""){
				ofLogVerbose("OnlineArtverts",artvertStr);
				Artvert artvert(artvertStr);
				if(!artvert.getCompressedImage().exists()){
					ofPtr<ofImage> icon = iconCache->getResource(artvert.getCompressedImage().getAbsolutePath()+"Resized");
					icon->setUseTexture(false);
					if(!icon->bAllocated()){
						ofLogVerbose("OnlineArtverts","trying to load remote icon " + url+"/"+artvertStr+".icon.jpg");
						icon->loadImage(url+"/"+artvertStr+".icon.jpg");
					}else{
						ofLogVerbose("OnlineArtverts",url+"/"+artvertStr+".icon.jpg already loaded");
					}
					ofPtr<gui::Button> button = ofPtr<gui::Button>(new gui::Button());
					button->setIcon(*icon);
					button->setFocusedIcon(*icon);
					button->setPressedIcon(*icon);
					ofAddListener(button->pressedE,this,&OnlineArtverts::artvertSelected);
					grid.addWidget(button);
					artverts.push_back(artvert);
				}else{
					ofLogVerbose("OnlineArtverts",artvertStr + " already downloaded");
				}
			}
		}
		if(grid.empty()){
			allDownloaded.setText("You've already downloaded\nall the artverts\nin " + currentCity);
		}
		grid.enableEvents();
		loading = false;
	}
}

void OnlineArtverts::listCountries(){
	state = Countries;
	list.disableEvents();
	loading = true;
	urlLoader.getAsync(url+"/listcountries.of","listcountries");
}

void OnlineArtverts::listCities(const string & country){
	state = Cities;
	list.disableEvents();
	loading = true;
	urlLoader.getAsync(url+"/listcities.of?country=" + country,"listcities");
}

void OnlineArtverts::listRoads(const string & country, const string & city){
	state = Roads;
	list.disableEvents();
	loading = true;
	urlLoader.getAsync(url+"/listroads.of?country=" +country+ "&city="+city ,"listroads");
}

void OnlineArtverts::listArtverts(const string & country, const string & city){
	state = Artverts;
	list.disableEvents();
	loading = true;
	urlLoader.getAsync(url+"/listartverts.of?country=" +country+ "&city="+city ,"listartverts");
}

void OnlineArtverts::countrySelected(const void * sender, bool & pressed){
	gui::Button* button = (gui::Button*)sender;
	currentCountry = button->getText();
	listCities(currentCountry);
}

void OnlineArtverts::citySelected(const void * sender, bool & pressed){
	gui::Button* button = (gui::Button*)sender;
	currentCity = button->getText();
	listArtverts(currentCountry,currentCity);
}

void OnlineArtverts::roadSelected(const void * sender, bool & pressed){
	/*state = Roads;
	gui::Button* button = (gui::Button*)sender;
	currentCity = button->getText();
	listRoads(currentCountry,currentCity);*/
}

void OnlineArtverts::artvertSelected(const void * sender, bool & pressed){
	for(int i=0;i<grid.size();i++){
		if(grid.getWidget(i).get()==sender){
			downloader.download(artverts[i]);
			ofSystemAlertDialog("Downloading artvert\nwill be available in the main menu once downloaded");
			return;
		}
	}
}


OnlineArtverts::DownloaderThread::DownloaderThread(OnlineArtverts * _parent){
	this->parent = _parent;
}

void OnlineArtverts::DownloaderThread::download(const Artvert & artvert){
	lock();
	pendingDownloads.push(artvert);
	unlock();
	if(!threadRunning){
		startThread(true,false);
	}else{
		condition.signal();
	}
}

void OnlineArtverts::DownloaderThread::threadedFunction(){
	while(isThreadRunning()){
		lock();
		if(!pendingDownloads.empty()){
			Artvert & artvert = pendingDownloads.front();
			parent->comm->downloadArtvert(artvert);
			ofNotifyEvent(parent->downloadedE,artvert);
			unlock();
		}else{
			unlock();
			condition.wait(mutex);
		}
	}
}

