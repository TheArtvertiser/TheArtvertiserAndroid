/*
 * TakeAPhoto.cpp
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#include "TakeAPhoto.h"

#include "ofAppRunner.h"
#include "ofGraphics.h"

TakeAPhoto::TakeAPhoto()
:video(NULL)
,state(Init)
,cameraButton(new gui::Button)
,yesButton(new gui::Button)
,noButton(new gui::Button)
,exitButton(new gui::Button)
,scale(1)
,videoWidth(0)
,videoHeight(0)
,pixelsCopied(false)
,address(new gui::Label)
{

}

void TakeAPhoto::setGeo(ofPtr<Geo> & _geo){
	geo = _geo;
}

void TakeAPhoto::setup(ofVideoGrabber & _video){
	video = &_video;

	quad.resize(4);
	quad[0].set(20,20);
	quad[1].set(620,20);
	quad[2].set(620,460);
	quad[3].set(20,460);

	warp.setMinDistance(ofGetWidth()*ofGetHeight()*.00015);

	photoIcon.loadImage("icons/camera.png");
	cameraButton->setIcon(photoIcon);
	ofAddListener(cameraButton->pressedE,this,&TakeAPhoto::takePhoto);

	yesIcon.loadImage("icons/yes.png");
	yesButton->setIcon(yesIcon);
	ofAddListener(yesButton->pressedE,this,&TakeAPhoto::yesPressed);

	noIcon.loadImage("icons/no.png");
	noButton->setIcon(noIcon);
	ofAddListener(noButton->pressedE,this,&TakeAPhoto::noPressed);

	exitIcon.loadImage("icons/exit.png");
	exitButton->setIcon(exitIcon);
	ofAddListener(exitButton->pressedE,this,&TakeAPhoto::exitPressed);

	ofResizeEventArgs window;
	window.width = ofGetWidth();
	window.height = ofGetHeight();
	windowResized(window);

	borderFrame.setWidth(yesIcon.getWidth()*0.5);
	borderFrame.setVSpacing(yesIcon.getHeight()*0.1);
	borderFrame.addWidget(cameraButton);
	borderFrame.disableEvents();

#ifdef TARGET_ANDROID
	ofxAndroidVideoGrabber * grabber = dynamic_cast<ofxAndroidVideoGrabber*>(_video.getGrabber().get());
	ofAddListener(grabber->newFrameE,this,&TakeAPhoto::newFrame);
#elif defined (TARGET_LINUX)
	ofGstVideoGrabber * grabber = dynamic_cast<ofGstVideoGrabber*>(_video.getGrabber().get());
	ofGstVideoUtils * videoUtils = grabber->getGstVideoUtils();
	ofAddListener(videoUtils->bufferEvent,this,&TakeAPhoto::newFrame);
#endif


	photo.setAnchorPercent(0.5,0.5);


	ofAddListener(ofEvents.windowResized,this,&TakeAPhoto::windowResized);

	geoPanel.setRectCompressed(ofRectangle(0,0,380,40));
	geoPanel.setRectExpanded(ofRectangle(0,0,380,320));
	geoPanel.setLeftMargin(20);
	geoPanel.expand();
	geoPanel.addWidget(address);
	geoPanel.addDrawable(geo);
	geoPanel.setVSpacing(15);
	geoPanel.enableEvents();

}

void TakeAPhoto::start(){
	updateState(Start);
}

void TakeAPhoto::stop(){
	updateState(Stop);
}

void TakeAPhoto::initWarp(){
	if(!video) return;
	scale = ofGetHeight()/video->getHeight();
	videoWidth = video->getWidth()*scale;
	videoHeight = video->getHeight()*scale;

	warp.setInitialQuad(quad,ofPoint(ofGetWidth()*0.5-videoWidth*0.5,ofGetHeight()*0.5-videoHeight*0.5),ofPoint(scale,scale));
}

void TakeAPhoto::windowResized(ofResizeEventArgs & window){
	ofLog(OF_LOG_ERROR,"TakeAPhoto window resized " + ofToString(window.width) + "," + ofToString(window.height));
	if(!video || video->getWidth()==0 || video->getHeight()==0) return;
	ofLog(OF_LOG_ERROR,"TakeAPhoto resizing");
	initWarp();

	int borderX = window.width*0.5 + videoWidth*0.5 + yesIcon.getWidth()*0.1;
	int borderY = yesIcon.getWidth()*0.1;
	borderFrame.setPosition(ofPoint(borderX,borderY));
}


void TakeAPhoto::updateState(Transition transition){
	if(transition==Stop){
		video->resetAnchor();
		borderFrame.disableEvents();
		state = Init;
		geo->stop();
		ofRemoveListener(ofEvents.touchDoubleTap,this,&TakeAPhoto::touchDoubleTap);
		bool yes;
		ofNotifyEvent(exitE,yes,this);
		return;
	}

	switch(state){
	case Init:
		if(transition==Start){
			borderFrame.clear();
			borderFrame.addWidget(cameraButton);
			borderFrame.addWidget(exitButton);
			borderFrame.enableEvents();
			video->setAnchorPercent(0.5,0.5);
			state = TakingPhoto;
			geo->setSize(320,240);
			geo->start();
			geoPanel.compress();
			ofAddListener(ofEvents.touchDoubleTap,this,&TakeAPhoto::touchDoubleTap);
		}

	case TakingPhoto:
		if(transition==PhotoPressed){
			borderFrame.clear();
			state = PhotoTaken;
		}
		break;

	case PhotoTaken:
		if(transition==UpdatedImage){
			warp.enableEvents();
			initWarp();
			borderFrame.addWidget(yesButton);
			borderFrame.addWidget(noButton);
			borderFrame.addWidget(exitButton);
			state = SelectingQuad;
		}
		break;

	case SelectingQuad:
		if(transition==YesPressed){
			warp.disableEvents();
			borderFrame.clear();
			borderFrame.addWidget(cameraButton);
			borderFrame.addWidget(exitButton);

			state = TakingPhoto;

			ofDirectory("adverts").create();
			string filename = ofGetTimestampString("%Y%m%d_%H%M%S%i");
			photo.saveImage("adverts/" + filename+".jpg",OF_IMAGE_QUALITY_BEST);

			ofFile roi("adverts/" + filename+".bmp.roi",ofFile::WriteOnly);
			for(int i=0;i<4;i++){
				roi << (int)warp.getQuad()[i].x << " " << (int)warp.getQuad()[i].y << endl;
			}
			roi.close();

			ofFile location("adverts/"+ filename + ".bmp.location",ofFile::WriteOnly);
			ofxLocation loc = geo->getLocation();
			location << loc;
			location.close();

			ofNotifyEvent(newPhotoE,filename,this);
		}
		if(transition==NoPressed){
			warp.disableEvents();
			borderFrame.clear();
			borderFrame.addWidget(cameraButton);
			borderFrame.addWidget(exitButton);
			state = TakingPhoto;
		}
		break;
	}
}

void TakeAPhoto::update(){

	if(videoWidth==0 || videoHeight==0){
		ofResizeEventArgs window;
		window.width = ofGetWidth();
		window.height = ofGetHeight();
		windowResized(window);
	}
	borderFrame.update();

	if(state==PhotoTaken && pixelsCopied){
		photo = photoPixels;
		pixelsCopied = false;
		updateState(UpdatedImage);
	}

	geo->update();
	address->setText(geo->getAddress());
	geoPanel.repositionWidgets();
	geoPanel.update();
}

void TakeAPhoto::draw(){
	ofPushStyle();
	ofSetColor(255);
	borderFrame.draw();
	if(state==SelectingQuad){
		ofSetColor(255);
		photo.draw(ofGetWidth()*0.5,ofGetHeight()*0.5,videoWidth,videoHeight);
		warp.draw();
	}else{
		ofSetColor(255);
		if(video) video->draw(ofGetWidth()*0.5,ofGetHeight()*0.5,videoWidth,videoHeight);

		//ofxLocation location = geo->getLocation();
		//ofDrawBitmapString(ofToString(location.altitude) + " " + ofToString(location.latitude) + " " + ofToString(location.longitude) + " " + ofToString(location.bearing),20,20);
		//ofDrawBitmapString(geo->getAddress(),20,20);
		ofSetColor(255,255,255,200);
		//geo->drawLastLocationImage(20,40);
		geoPanel.draw();
		ofPopStyle();
	}


}

TakeAPhoto::State TakeAPhoto::getState(){
	return state;
}

ofImage & TakeAPhoto::getPhoto(){
	return photo;
}

void TakeAPhoto::takePhoto(bool & pressed){
	updateState(PhotoPressed);
}

void TakeAPhoto::yesPressed(bool & pressed){
	updateState(YesPressed);
}

void TakeAPhoto::noPressed(bool & pressed){
	updateState(NoPressed);
}

void TakeAPhoto::exitPressed(bool & pressed){
	updateState(Stop);
}

void TakeAPhoto::newFrame(ofPixels & newFrame){
	if(state==TakingPhoto && video){
		photoPixels = video->getPixelsRef();
		pixelsCopied = true;
	}
}

void TakeAPhoto::touchDoubleTap(ofTouchEventArgs & touch){
	geo->increaseZoom();
}
