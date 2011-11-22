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

#include "ArtvertiserApp.h"
#include "Artvert.h"
#include "PersistanceEngine.h"
#include "Label.h"


int camW = 640;
int camH = 480;

//--------------------------------------------------------------
void ArtvertiserApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);

	//ofSleepMillis(5000);
	ofSetVerticalSync(true);
	ofBackground(66,51,51);
	ofEnableAlphaBlending();

	vector<Artvert> artverts = Artvert::listAll();
	ofLogVerbose("ArtvertiserApp","checking artverts integrity");
	for(int i=0; i<(int)artverts.size(); i++){
		if(!artverts[i].checkIntegrity()){
			ofLogVerbose("ArtvertiserApp") << "found corrupt or incomplete advert " << artverts[i].getUID() << " deleting";
			string generated = artverts[i].generateMD5();
			string stored = artverts[i].getStoredMD5();
			ofLogVerbose("ArtvertiserApp") << "generated md5 " << artverts[i].generateMD5();
			ofLogVerbose("ArtvertiserApp") << "stored    md5 " << artverts[i].getStoredMD5();
			artverts[i].removeAnalisys();
			ofFile md5(artverts[i].getMD5File().path(),ofFile::WriteOnly);
			md5 << artverts[i].generateMD5();
			md5.close();
		}
	}

	refreshArtvert = false;

	ofLogVerbose("ArtvertiserApp","setting orientation and screen sleep lock");
#ifdef TARGET_ANDROID
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	ofxAndroidLockScreenSleep();
#endif

	ofLogVerbose("ArtvertiserApp","init video grabber");
	//ofSleepMillis(5000);
	//grabber.setDeviceID(1);
	//grabber.setDesiredFrameRate(60);
	//grabber.setUseTexture(false);

	//grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.initGrabber(camW, camH);

#ifdef TARGET_ANDROID
	ofLogVerbose("ArtvertiserApp","set audofocus");
	ofxAndroidVideoGrabber * androidGrabber = (ofxAndroidVideoGrabber *) grabber.getGrabber().get();
	if(!androidGrabber->setAutoFocus(true)){
		ofSystemAlertDialog("Couldn't activate autofocus");
	}
#endif


	ofLogVerbose("ArtvertiserApp","Create gui...");
	counter = 0;
	allocated = true;

	iconCache = ofPtr<gui::IconCache>(new gui::IconCache);

	geo = ofPtr<ofxGoogleMaps>(new ofxGoogleMaps);

	comm = ofPtr<Comm>(new Comm);

	ofLogVerbose("ArtvertiserApp","Inject dependencies");
	menu.setIconCache(iconCache);
	menu.setup();
	menu.enableEvents();

	artvertInfo.setIconCache(iconCache);
	artvertInfo.setGeo(geo);

	takeAPhoto.setGeo(geo);
	takeAPhoto.setup(grabber);

	onlineArtverts.setURL("http://192.168.1.134:8888");
	onlineArtverts.setIconCache(iconCache);
	onlineArtverts.setComm(comm);
	onlineArtverts.setup();

	comm->setURL("http://192.168.1.134:8888");
	comm->start();



	circularPB.setRadius(30);
	circularPB.setColor(ofColor(190,190,190));
	circularPB.setPosition(ofPoint(grabber.getWidth()*0.5,ofGetHeight()*0.5));


	imgQuad.resize(4);

	ofLogVerbose("ArtvertiserApp","Load settings");

	settings.loadFile("config/settings.xml");
	settings.pushTag("settings");
	showDebugInfo = settings.getValue("debug",0);
	if(settings.getValue("kiosk",0)){
		state = Kiosk;
		ofLogVerbose("ArtvertiserApp","set artvert changed event");
		ofAddListener(kioskMode.artvertChangedE,this,&ArtvertiserApp::artvertSelected);

		ofLogVerbose("ArtvertiserApp","setup kiosk mode");
		kioskMode.setup(settings.getValue("secsartvert",60), settings.getValue("artvert","20110504_015507715"));

		ofLogVerbose("ArtvertiserApp","setup detector");
		Artvert & artvert = kioskMode.getArtvert();
		if(artvert.hasAlias()){
			artvertiser.setup(artvert.getAlias().getModel().getAbsolutePath(),grabber,imgQuad);
		}else{
			artvertiser.setup(artvert.getModel().getAbsolutePath(),grabber,imgQuad);
		}

	}else{
		state = Menu;
		ofAddListener(takeAPhoto.exitE,this,&ArtvertiserApp::appFinished);
		ofAddListener(takeAPhoto.newPhotoE,this,&ArtvertiserApp::newPhoto);

		ofAddListener(comm->gotAnalysisE,this,&ArtvertiserApp::gotAnalysis);

		ofAddListener(menu.cameraPressedE,this,&ArtvertiserApp::cameraPressed);
		ofAddListener(menu.downloadPressedE,this,&ArtvertiserApp::downloadPressed);
		ofAddListener(menu.artvertSelectedE,this,&ArtvertiserApp::advertSelected);

		ofAddListener(artvertInfo.artvertSelectedE,this,&ArtvertiserApp::artvertSelected);


		ofAddListener(onlineArtverts.downloadedE,this,&ArtvertiserApp::gotAnalysis);


		ofAddListener(avahi.serviceNewE,this,&ArtvertiserApp::avahiServiceFound);
		//avahi.lookup("_services._dns-sd._udp");
		avahi.lookup("_http._tcp");
	}

	ofLogVerbose("ArtvertiserApp","End setup");
}

//--------------------------------------------------------------
void ArtvertiserApp::update(){
	static enum State prev_state = state;
   if ( prev_state != state )
   {
	   string state_string = "unknown";
	   if ( state == Menu )
		   state_string = "Menu";
	   else if ( state == Photo )
		   state_string = "Photo";
	   else if ( state == OnlineList )
		   state_string = "OnlineList";
	   else if ( state == Info )
		   state_string = "Info";
	   else if ( state == Tracking )
		   state_string = "Tracking";
	   ofLogVerbose("ArtvertiserApp", "state changed: now "+ state_string );
	   prev_state = state;
   }	   
	switch(state){
	case Menu:
		if(refreshMenu){
			menu.refresh();
			menu.enableEvents();
			refreshMenu = false;
		}
		menu.update();
		break;
	case Photo:
		grabber.update();
		takeAPhoto.update();
		break;
	case OnlineList:
		onlineArtverts.update();
		break;
	case Info:
		artvertInfo.update();
		break;
	case Kiosk:
		kioskMode.update();
	case Tracking:
		if(refreshArtvert){
			subs_img.setUseTexture(true);
			subs_img.reloadTexture();
			refreshArtvert = false;
		}
		if(artvertiser.getState()!=Detector::Initializing){
			grabber.update();
		}else{
			circularPB.update();
		}

		break;
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::draw(){

	int w=camW, h=camH;
	int x;

	switch(state){
	case Menu:
		menu.draw();
		ofDrawBitmapString(comm->getURL(),ofGetWidth()-300,ofGetHeight()-20);
		break;
	case Photo:
		takeAPhoto.draw();
		break;
	case OnlineList:
		onlineArtverts.draw();
		ofDrawBitmapString(comm->getURL(),ofGetWidth()-300,ofGetHeight()-20);
		break;
	case Info:
		artvertInfo.draw();
		break;
	case Kiosk:
	case Tracking:
		ofSetHexColor(0xFFFFFF);
		float scale=1;
		h = ofGetHeight();
		w = float(camW)/float(camH)*float(h);
		x = (ofGetWidth() - w)/2;
		scale = float(w)/float(camW);
		if(artvertiser.getState()!=Detector::Initializing){
			grabber.draw(x,0,w,h);
		}else{
			circularPB.draw();
		}
		if(artvertiser.isDetected() || artvertiser.isTracked()){
			ofPushMatrix();
			ofTranslate(x,0);
			ofScale(scale,scale,1);
			glMultMatrixf(artvertiser.getHomography().getPtr());
			subs_img.draw(0,0);
			ofPopMatrix();
		}
		if(showDebugInfo)ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), x+20, 20);
		if(showDebugInfo)ofDrawBitmapString("detect fps: " + ofToString(artvertiser.getFps()), x+20, 40);

		if(artvertiser.getState()==Detector::Initializing){
			if(showDebugInfo) ofDrawBitmapString("Initializing", x+20, 60);
		}else if(artvertiser.isDetected()){
			if(showDebugInfo) ofDrawBitmapString("Detected", x+20, 60);
		}else if(artvertiser.isTracked()){
			if(showDebugInfo) ofDrawBitmapString("Tracked", x+20, 60);
		}else{
			if(showDebugInfo) ofDrawBitmapString("NotDetected", x+20, 60);
		}

		if(!allocated) ofDrawBitmapString("warning: not allocated", x+20, 80);
		if(state==Kiosk && showDebugInfo) ofDrawBitmapString(kioskMode.getCurrentArtvertName() + ", next in: " + ofToString(kioskMode.getSecsNextArtvert()), x+20,80);
		break;
	}
}


//--------------------------------------------------------------
void ArtvertiserApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void ArtvertiserApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ArtvertiserApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ArtvertiserApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ArtvertiserApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ArtvertiserApp::windowResized(int w, int h){
	circularPB.setRadius(30);
	circularPB.setPosition(ofPoint(grabber.getWidth()*0.5,h*0.5));
}

//--------------------------------------------------------------

void ArtvertiserApp::newPhoto(const void * sender, Artvert & artvert ){
	ofLogVerbose("ArtvertiserApp") << "new photo: " << artvert.getUID();
	if(sender==&takeAPhoto){

		artvert.save();
		PersistanceEngine::save();

		ofLogVerbose("ArtvertiserApp") << "sender correct";

		comm->sendAdvert(artvert);
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::appFinished(const void * sender,bool & finished){
	if(sender==&takeAPhoto){
		refreshMenu = true;
		state = Menu;
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::gotAnalysis(const Artvert & artvert){
	if(state==Menu){
		refreshMenu = true;
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::cameraPressed(bool & pressed){
	state = Photo;
	takeAPhoto.start();
	menu.disableEvents();
}

//--------------------------------------------------------------
void ArtvertiserApp::downloadPressed(bool & pressed){
	state = OnlineList;
	onlineArtverts.start();
	menu.disableEvents();
}

//--------------------------------------------------------------
void ArtvertiserApp::artvertSelected(ofFile & artvertimg){
	ofLogVerbose("ArtvertiserApp") << "artvert slelected" << artvertimg.getFileName();
	ofLogVerbose("ArtvertiserApp") << "allocating image";

	subs_img.setUseTexture(false);
	subs_img.loadImage(artvertimg);
	if(!subs_img.bAllocated()) allocated = false;
	refreshArtvert = true;

	ofLogVerbose("ArtvertiserApp") << "setting quad";

	imgQuad[0].set(0,0);
	imgQuad[1].set(subs_img.getWidth(),0);
	imgQuad[2].set(subs_img.getWidth(),subs_img.getHeight());
	imgQuad[3].set(0,subs_img.getHeight());

	if(state!=Kiosk){
		state = Tracking;
		artvertInfo.stop();
		Artvert & artvert = artvertInfo.getCurrentArtvert();
		if(artvert.hasAlias()){
			artvertiser.setup(artvert.getAlias().getModel().getAbsolutePath(),grabber,imgQuad);
		}else{
			artvertiser.setup(artvert.getModel().getAbsolutePath(),grabber,imgQuad);
		}
	}else{
		ofLogVerbose("ArtvertiserApp") << "setting detector quad";
		artvertiser.setSrcQuad(imgQuad);
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::advertSelected(Artvert & artvert){
	state = Info;
	artvertInfo.show(artvert);
	menu.disableEvents();
}

//--------------------------------------------------------------
void ArtvertiserApp::avahiServiceFound(ofxAvahiService & service){
	ofLogNotice("ArtvertiserApp") << "new service" << service.name;
	if(service.name=="Artvertiser server"){
		string url = "http://"+service.ip+":"+ofToString(service.port);
		ofLogNotice("ArtvertiserApp") << "found artvertiser server:" << url;
		comm->setURL(url);
		onlineArtverts.setURL(url);
	}
}

//--------------------------------------------------------------
bool ArtvertiserApp::backPressed(){
	switch(state){
	case Menu:
		return false;
		break;
	case OnlineList:
		if(!onlineArtverts.back()){
			onlineArtverts.stop();
			menu.refresh();
			menu.enableEvents();
			state = Menu;
		}
		return true;
		break;
	case Photo:
		takeAPhoto.stop();
		return true;
		break;
	case Tracking:
		artvertiser.close();
		state = Info;
		artvertInfo.show(artvertInfo.getCurrentArtvert());
		return true;
	case Info:
		artvertInfo.stop();
		menu.refresh();
		menu.enableEvents();
		state = Menu;
		return true;
		break;
	case Kiosk:
		return true;  // ignore back key
		break;
	}
	return false;
}

