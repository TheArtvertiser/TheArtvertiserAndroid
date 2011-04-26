#include "ArtvertiserApp.h"
#include "Artvert.h"
#include "PersistanceEngine.h"

#define LIVE_VIDEO
int camW = 640;
int camH = 480;

//--------------------------------------------------------------
void ArtvertiserApp::setup(){
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(66,51,51);
	ofEnableAlphaBlending();

	refreshMenu = false;

#ifdef TARGET_ANDROID
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
#endif

#ifdef LIVE_VIDEO
	grabber.setDeviceID(1);
	grabber.setDesiredFrameRate(60);
	grabber.setUseTexture(false);
	//grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.initGrabber(camW, camH);
#endif
	counter = 0;
	allocated = true;
	ofLog(OF_LOG_WARNING,"loading " + ofToDataPath("model"+ofToString(counter)+".jpg"));
	//ofimg.loadImage("model1.bmp");
	//if(!ofimg.bAllocated()) allocated = false;
	subs_img.loadImage("default_artvert.bmp");
	if(!subs_img.bAllocated()) allocated = false;

	imgQuad.resize(4);
	imgQuad[0].set(0,0);
	imgQuad[1].set(subs_img.getWidth(),0);
	imgQuad[2].set(subs_img.getWidth(),subs_img.getHeight());
	imgQuad[3].set(0,subs_img.getHeight());

	menu.setup();
	menu.enableEvents();

	//artvertiser.setup("model1.bmp",grabber,imgQuad);
	takeAPhoto.setup(*grabber.getGrabber());


	state = Menu;

	comm.setURL("http://192.168.1.35:8888");
	comm.start();

	ofAddListener(takeAPhoto.exitE,this,&ArtvertiserApp::appFinished);
	ofAddListener(takeAPhoto.newPhotoE,this,&ArtvertiserApp::newPhoto);
	ofAddListener(comm.gotAnalysisE,this,&ArtvertiserApp::gotAnalysis);
	ofAddListener(menu.cameraPressedE,this,&ArtvertiserApp::cameraPressed);
	ofAddListener(menu.artvertSelectedE,this,&ArtvertiserApp::artvertSelected);
}

//--------------------------------------------------------------
void ArtvertiserApp::update(){
	switch(state){
	case Menu:
		if(refreshMenu){
			menu.refresh();
			refreshMenu = false;
		}
		menu.update();
		break;
	case Photo:
		grabber.update();
		takeAPhoto.update();
		break;
	case Tracking:
		grabber.update();
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
	case Tracking:
		ofSetHexColor(0xFFFFFF);
		if(artvertiser.getState()!=Detector::Initializing){
			grabber.draw(0,0,640,480);
		}
		if(artvertiser.isDetected() || artvertiser.isTracked()){
			ofPushMatrix();
			glMultMatrixf(artvertiser.getHomography().getPtr());
			subs_img.draw(0,0);
			ofPopMatrix();
		}
		ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), 660, 20);
		ofDrawBitmapString("detect fps: " + ofToString(artvertiser.getFps()), 660, 40);

		if(artvertiser.getState()==Detector::Initializing){
			ofDrawBitmapString("Initializing", 660, 60);
		}else if(artvertiser.isDetected()){
			ofDrawBitmapString("Detected", 660, 60);
		}else if(artvertiser.isTracked()){
			ofDrawBitmapString("Tracked", 660, 60);
		}else{
			ofDrawBitmapString("NotDetected", 660, 60);
		}

		if(!allocated) ofDrawBitmapString("warning: not allocated", 660, 80);
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

}

//--------------------------------------------------------------

void ArtvertiserApp::newPhoto(const void * sender, string & filename ){
	if(sender==&takeAPhoto){
		Artvert artvert(filename);
		artvert.save();
		PersistanceEngine::save();

		comm.sendAdvert(artvert);
		takeAPhoto.getPhoto().saveImage("adverts/"+filename+".bmp");
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::appFinished(const void * sender,bool & finished){
	if(sender==&takeAPhoto){
		menu.refresh();
		menu.enableEvents();
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
void ArtvertiserApp::artvertSelected(Artvert & artvert){
	if(artvert.isReady()){
		state = Tracking;
		menu.disableEvents();
		if(artvert.hasAlias()){
			artvertiser.setup(artvert.getAlias().getModel().getAbsolutePath(),grabber,imgQuad);
		}else{
			artvertiser.setup(artvert.getModel().getAbsolutePath(),grabber,imgQuad);
		}
	}else{

	}
}

//--------------------------------------------------------------
bool ArtvertiserApp::backPressed(){
	switch(state){
	case Menu:
		return false;
		break;
	case Photo:
		takeAPhoto.stop();
		break;
	case Tracking:
		artvertiser.close();
		menu.enableEvents();
		state = Menu;
		return true;
		break;
	}
	return false;
}
