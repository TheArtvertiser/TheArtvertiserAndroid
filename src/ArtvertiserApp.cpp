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
 along with The Artvertis	er for Android.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ArtvertiserApp.h"
#include "Artvert.h"
#include "PersistanceEngine.h"
#include "Label.h"
#include "Binocular.h"
#include "CommandlineParser.h"

//void ofSoundShutdown(){};

int camW = 800;
int camH = 600;
int detectW = 640;
int detectH = 480;
//int detectW = -1;
//int detectH = -1;

//static const string SERVER_URL = "http://192.168.1.134:8888";
static const string SERVER_URL = "http://localhost:8888";
//static const string SERVER_URL = "http://192.168.1.4:8888";

//--------------------------------------------------------------
void ArtvertiserApp::setup(){
	//ofSleepMillis(5000);
	ofSetVerticalSync(true);
//	ofSetFrameRate( 30.0f );
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(66,51,51);
	ofEnableAlphaBlending();

	ofSetLogLevel("ArtvertiserApp",OF_LOG_VERBOSE);
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

#ifdef TARGET_ANDROID
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
#endif

	grabber.setDeviceID(0);
	grabber.setDesiredFrameRate(60);
	grabber.setUseTexture(false);
	//grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.initGrabber(camW, camH);
	#ifdef TARGET_LINUX
		ofLogNotice("ArtvertiserApp", "sleeping 10s to allow camera to recover");
	// sleep to allow the camera to recover from init
		sleep(10);
	#endif



	// drop the first few frames
	int dropFrameCount = 0;
	while ( dropFrameCount < 10 )
	{
		grabber.update();
		if ( !grabber.isFrameNew() )
			usleep( 1000*10 );
		else
		{
	//		const ofPixels& p = grabber.getPixelsRef();
			ofLogVerbose("ArtvertiserApp") << "dropping frame "/* << p.getWidth() << "x" << p.getHeight()*/;
			dropFrameCount++;
		}
	}
	
	

	if ( CommandlineParser::get()->isRunningOnBinoculars() )
	{
		Binocular::get()->setup( grabber, /*bDebug*/ true );
	}

	counter = 0;
	allocated = true;

	iconCache = ofPtr<gui::IconCache>(new gui::IconCache);

	geo = ofPtr<ofxGeoLocation>(new ofxGeoLocation);

	comm = ofPtr<Comm>(new Comm);

	menu.setIconCache(iconCache);
	menu.setup();
	menu.enableEvents();

	artvertInfo.setIconCache(iconCache);
	artvertInfo.setGeo(geo);

	takeAPhoto.setGeo(geo);
	takeAPhoto.setup(grabber);

	onlineArtverts.setURL(SERVER_URL);
	onlineArtverts.setIconCache(iconCache);
	onlineArtverts.setComm(comm);
	onlineArtverts.setup();

	comm->setURL(SERVER_URL);
	comm->start();

	if ( CommandlineParser::get()->isRunningOnBinoculars() )
	{
		state = Tracking;
	}
	else
	{
		state = Menu;
	}


	circularPB.setRadius(30);
	circularPB.setColor(ofColor(190,190,190));
	circularPB.setPosition(ofPoint(grabber.getWidth()*0.5,ofGetHeight()*0.5));


	imgQuad.resize(4);

	ofAddListener(takeAPhoto.exitE,this,&ArtvertiserApp::appFinished);
	ofAddListener(takeAPhoto.newPhotoE,this,&ArtvertiserApp::newPhoto);

	ofAddListener(comm->gotAnalysisE,this,&ArtvertiserApp::gotAnalysis);

	ofAddListener(menu.cameraPressedE,this,&ArtvertiserApp::cameraPressed);
	ofAddListener(menu.downloadPressedE,this,&ArtvertiserApp::downloadPressed);
	ofAddListener(menu.artvertSelectedE,this,&ArtvertiserApp::advertSelected);

	ofAddListener(artvertInfo.artvertSelectedE,this,&ArtvertiserApp::artvertSelected);


	ofAddListener(onlineArtverts.downloadedE,this,&ArtvertiserApp::gotAnalysis);

	if ( CommandlineParser::get()->isRunningOnBinoculars() )
	{
		ofAddListener( Binocular::get()->artvertSelectedEvent, this, &ArtvertiserApp::artvertSelectedBinoculars );
	}

	
	ofSetLogLevel(OF_LOG_VERBOSE);
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
	case Tracking:
		if(refreshArtvert){
			subs_img.setUseTexture(true);
			subs_img.reloadTexture();
			refreshArtvert = false;
		}
		if(artvertiser.getState()!=Detector::Initializing){
			
			grabber.update();
#ifdef TARGET_OSX
			if ( grabber.isFrameNew() )
			{
				artvertiser.newFrame( grabber.getPixelsRef() );
				Binocular::get()->gotFrame( grabber.getPixelsRef() );
			}
#endif
			
		}else{
			circularPB.update();
		}

		break;
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::draw(){
	switch(state){
	case Menu:
		menu.draw();
		break;
	case Photo:
		takeAPhoto.draw();
		break;
	case OnlineList:
		onlineArtverts.draw();
		break;
	case Info:
		artvertInfo.draw();
		break;
	case Tracking:
		ofSetHexColor(0xFFFFFF);
		int x=0, w=camW, h=camH;
		float scale=1;
		if(artvertiser.getState()!=Detector::Initializing){
			h = ofGetHeight();
			w = float(camW)/float(camH)*float(h);
			x = (ofGetWidth() - w)/2;
			scale = float(w)/float(camW);
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
		ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), x+20, 20);
		ofDrawBitmapString("detect fps: " + ofToString(artvertiser.getFps()), x+20, 40);

		if(artvertiser.getState()==Detector::Initializing){
			ofDrawBitmapString("Initializing", x+20, 60);
		}else if(artvertiser.isDetected()){
			ofDrawBitmapString("Detected", x+20, 60);
		}else if(artvertiser.isTracked()){
			ofDrawBitmapString("Tracked", x+20, 60);
		}else{
			ofDrawBitmapString("NotDetected", x+20, 60);
		}

		if(!allocated) ofDrawBitmapString("warning: not allocated", x+20, 80);
		break;
	}
}


//--------------------------------------------------------------
void ArtvertiserApp::keyPressed  (int key){
	if ( key == OF_KEY_BACKSPACE )
		backPressed();
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
	subs_img.setUseTexture(false);
	subs_img.loadImage(artvertimg);
	if(!subs_img.bAllocated()) allocated = false;
	refreshArtvert = true;

	imgQuad[0].set(0,0);
	imgQuad[1].set(subs_img.getWidth(),0);
	imgQuad[2].set(subs_img.getWidth(),subs_img.getHeight());
	imgQuad[3].set(0,subs_img.getHeight());

	state = Tracking;
	artvertInfo.stop();
	Artvert & artvert = artvertInfo.getCurrentArtvert();

	if(artvert.hasAlias()){
		artvertiser.setup(artvert.getAlias().getModel().getAbsolutePath(),grabber,imgQuad, false, detectW, detectH );
		ofLogVerbose("ArtvertiserApp", "artvert.hasAlias()");
	}else{
		ofLogVerbose("ArtvertiserApp", "doesn't: artvert.hasAlias()");
		artvertiser.setup(artvert.getModel().getAbsolutePath(),grabber,imgQuad, false, detectW, detectH );
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::advertSelected(Artvert & artvert){
	state = Info;
	artvertInfo.show(artvert);
	menu.disableEvents();
}

void ArtvertiserApp::artvertSelectedBinoculars(Binocular::ArtvertSelectedEventInfo &info)
{
	subs_img.setUseTexture(false);
	subs_img.loadImage( info.selectedArtvert );
	if ( !subs_img.bAllocated()) allocated = false;
	refreshArtvert = true;
	
	imgQuad[0].set(0,0);
	imgQuad[1].set(subs_img.getWidth(),0);
	imgQuad[2].set(subs_img.getWidth(),subs_img.getHeight());
	imgQuad[3].set(0,subs_img.getHeight());
	
	state = Tracking;
	Artvert & artvert = info.selectedAdvert;
	if ( artvert.hasAlias() )
	{
		artvertiser.setup(artvert.getAlias().getModel().getAbsolutePath(),grabber,imgQuad, false, detectW, detectH );
		ofLogVerbose("ArtvertiserApp", "artvert.hasAlias()");
	}else{
		ofLogVerbose("ArtvertiserApp", "doesn't: artvert.hasAlias()");
		artvertiser.setup(artvert.getModel().getAbsolutePath(),grabber,imgQuad, false, detectW, detectH );
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
	}
	return false;
}
