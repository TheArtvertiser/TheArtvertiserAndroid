#include "testApp.h"

#define LIVE_VIDEO
int camW = 640;
int camH = 480;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(255,255,255);
	ofEnableAlphaBlending();

#ifdef TARGET_ANDROID
	ofToggleFullscreen();
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
#endif

#ifdef LIVE_VIDEO
	grabber.setDeviceID(1);
	grabber.setDesiredFrameRate(60);
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

	//artvertiser.setup("model1.bmp",grabber,imgQuad);
	takeAPhoto.setup(grabber);
	ofAddListener(takeAPhoto.finishedE,this,&testApp::appFinished);
	ofAddListener(comm.gotAnalysisE,this,&testApp::gotAnalysis);
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	takeAPhoto.update();
	comm.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	if(takeAPhoto.getState()!=TakeAPhoto::Finished){
		takeAPhoto.draw();
	}else{
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
		ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 660, 20);
		ofDrawBitmapString("detection fps: "+ofToString(artvertiser.getFps()), 660, 40);

		if(artvertiser.getState()==Detector::Initializing){
			ofDrawBitmapString("Initializing", 660, 60);
		}else if(artvertiser.isDetected()){
			ofDrawBitmapString("Detected", 660, 60);
		}else if(artvertiser.isTracked()){
			ofDrawBitmapString("Tracked", 660, 60);
		}else{
			ofDrawBitmapString("NotDetected", 660, 60);
		}

		if(!allocated)ofDrawBitmapString("warning: not allocated", 660, 80);
	}



}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::appFinished(const void * sender, string & filename){
	if(sender==&takeAPhoto){
		comm.setURL("http://192.168.1.35:8888");
		comm.sendAdvert(filename);
		takeAPhoto.getPhoto().saveImage("adverts/"+filename+".bmp");
	}
}

//--------------------------------------------------------------
void testApp::gotAnalysis(string & filename){
	cout << "starting ferns" << endl;
	artvertiser.setup("adverts/"+filename+".bmp",grabber,imgQuad);
}

