#include "ArtvertiserApp.h"
#include "Artvert.h"
#include "PersistanceEngine.h"

int camW = 640;
int camH = 480;

//--------------------------------------------------------------
void ArtvertiserApp::setup(){
	ofSetVerticalSync(true);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(66,51,51);
	ofEnableAlphaBlending();

	refreshArtvert = false;

#ifdef TARGET_ANDROID
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
#endif

	grabber.setDeviceID(1);
	grabber.setDesiredFrameRate(60);
	grabber.setUseTexture(false);
	//grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.initGrabber(camW, camH);

	counter = 0;
	allocated = true;
	ofLog(OF_LOG_WARNING,"loading " + ofToDataPath("model"+ofToString(counter)+".jpg"));
	//ofimg.loadImage("model1.bmp");
	//if(!ofimg.bAllocated()) allocated = false;

	iconCache = ofPtr<gui::IconCache>(new gui::IconCache);

	menu.setIconCache(iconCache);
	menu.setup();
	menu.enableEvents();

	geo = ofPtr<Geo>(new Geo);

	artvertInfo.setIconCache(iconCache);
	artvertInfo.setGeo(geo);

	takeAPhoto.setGeo(geo);
	takeAPhoto.setup(grabber);


	state = Menu;

	comm.setURL("http://192.168.1.35:8888");
	comm.start();

	circularPB.setRadius(30);
	circularPB.setPosition(ofPoint(grabber.getWidth()*0.5,ofGetHeight()*0.5));


	imgQuad.resize(4);

	ofAddListener(takeAPhoto.exitE,this,&ArtvertiserApp::appFinished);
	ofAddListener(takeAPhoto.newPhotoE,this,&ArtvertiserApp::newPhoto);
	ofAddListener(comm.gotAnalysisE,this,&ArtvertiserApp::gotAnalysis);
	ofAddListener(menu.cameraPressedE,this,&ArtvertiserApp::cameraPressed);
	ofAddListener(artvertInfo.artvertSelectedE,this,&ArtvertiserApp::artvertSelected);
	ofAddListener(menu.artvertSelectedE,this,&ArtvertiserApp::advertSelected);
}

//--------------------------------------------------------------
void ArtvertiserApp::update(){
	switch(state){
	case Menu:
		menu.update();
		break;
	case Photo:
		grabber.update();
		takeAPhoto.update();
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
	case Info:
		artvertInfo.draw();
		break;
	case Tracking:
		ofSetHexColor(0xFFFFFF);
		if(artvertiser.getState()!=Detector::Initializing){
			grabber.draw(0,0,640,480);
		}else{
			circularPB.draw();
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
	circularPB.setRadius(30);
	circularPB.setPosition(ofPoint(grabber.getWidth()*0.5,h*0.5));
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
		menu.refresh();
		menu.enableEvents();
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::cameraPressed(bool & pressed){
	state = Photo;
	takeAPhoto.start();
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
		artvertiser.setup(artvert.getAlias().getModel().getAbsolutePath(),grabber,imgQuad);
	}else{
		artvertiser.setup(artvert.getModel().getAbsolutePath(),grabber,imgQuad);
	}
}

//--------------------------------------------------------------
void ArtvertiserApp::advertSelected(Artvert & artvert){
	state = Info;
	artvertInfo.show(artvert);
	menu.disableEvents();
}

//--------------------------------------------------------------
bool ArtvertiserApp::backPressed(){
	switch(state){
	case Menu:
		return false;
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
