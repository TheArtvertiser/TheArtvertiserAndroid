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
,pixelsCopied(false)
{

}

void TakeAPhoto::setup(ofBaseVideo & _video){
	video = &_video;

	quad.resize(4);
	quad[0].set(20,20);
	quad[1].set(620,20);
	quad[2].set(620,460);
	quad[3].set(20,460);

	warp.setMinDistance(ofGetWidth()*ofGetHeight()*.00015);
	warp.setInitialQuad(quad,ofPoint(ofGetWidth()*0.5-video->getWidth()*0.5,ofGetHeight()*0.5-video->getHeight()*0.5));

	photoIcon.loadImage("icons/camera.png");
	photoButton.setIcon(photoIcon);
	ofAddListener(photoButton.pressedE,this,&TakeAPhoto::takePhoto);

	yesIcon.loadImage("icons/yes.png");
	yesButton.setIcon(yesIcon);
	ofAddListener(yesButton.pressedE,this,&TakeAPhoto::yesPressed);

	noIcon.loadImage("icons/no.png");
	noButton.setIcon(noIcon);
	ofAddListener(noButton.pressedE,this,&TakeAPhoto::noPressed);


	warp.disableEvents();
	yesButton.disableEvents();
	noButton.disableEvents();
	photoButton.enableEvents();

#ifdef TARGET_ANDROID
	ofxAndroidVideoGrabber * grabber = dynamic_cast<ofxAndroidVideoGrabber*>(video);
	ofAddListener(grabber->newFrameE,this,&TakeAPhoto::newFrame);
#elif defined (TARGET_LINUX)
	ofGstVideoGrabber * grabber = dynamic_cast<ofGstVideoGrabber*>(video);
	ofGstVideoUtils * videoUtils = grabber->getGstVideoUtils();
	ofAddListener(videoUtils->bufferEvent,this,&TakeAPhoto::newFrame);
#endif

	videoTex.allocate(video->getWidth(),video->getHeight(),GL_RGB);

	photo.setAnchorPercent(0.5,0.5);
	videoTex.setAnchorPercent(0.5,0.5);


	photoButton.setRect(borderButton);
	yesButton.setRect(yesCenter);
	noButton.setRect(noCenter);

	ofAddListener(ofEvents.windowResized,this,&TakeAPhoto::windowResized);
}

void TakeAPhoto::windowResized(ofResizeEventArgs & window){
	int centerLX = window.width*0.5 - yesIcon.getWidth() - yesIcon.getWidth()*0.4;
	int centerRX = window.width*0.5 + yesIcon.getWidth()*0.4;
	int borderX = window.width*0.5 + video->getWidth()*0.5 + yesIcon.getWidth()*0.1;

	yesCenter.set(centerLX,190,128,100);
	noCenter.set(centerRX,190,100,104);
	borderButton.set(borderX,200,56,35);

	photoButton.setRect(borderButton);
	yesButton.setRect(yesCenter);
	noButton.setRect(noCenter);

	quad.resize(4);
	quad[0].set(20,20);
	quad[1].set(620,20);
	quad[2].set(620,460);
	quad[3].set(20,460);

	warp.setMinDistance(ofGetWidth()*ofGetHeight()*.00015);
	warp.setInitialQuad(quad,ofPoint(ofGetWidth()*0.5-video->getWidth()*0.5,ofGetHeight()*0.5-video->getHeight()*0.5));

}



void TakeAPhoto::updateState(Transition transition){
	switch(state){
	case Init:
		if(transition==PhotoPressed){
			warp.disableEvents();
			yesButton.setRect(yesCenter);
			yesButton.enableEvents();
			noButton.enableEvents();
			photoButton.disableEvents();

			state = TakingPhoto;
		}
		break;

	case TakingPhoto:
		if(transition==UpdatedImage){
			warp.disableEvents();
			yesButton.enableEvents();
			noButton.enableEvents();
			photoButton.disableEvents();

			state = PhotoTaken;
		}
		break;

	case PhotoTaken:
		if(transition==YesPressed){
			yesButton.enableEvents();
			noButton.disableEvents();
			photoButton.disableEvents();
			yesButton.setRect(borderButton);
			warp.enableEvents();

			state = SelectingQuad;
		}
		if(transition==NoPressed){
			warp.disableEvents();
			yesButton.disableEvents();
			noButton.disableEvents();
			photoButton.enableEvents();

			state = Init;
		}
		break;

	case SelectingQuad:
		if(transition==YesPressed){
			warp.disableEvents();
			yesButton.disableEvents();
			noButton.disableEvents();
			photoButton.enableEvents();

			state = Finished; //Loop state, surely later this should just finish
			ofDirectory("adverts").create();
			string filename = ofGetTimestampString("%Y%m%d%_H%M%S%i");
			photo.saveImage("adverts/" + filename+".jpg",OF_IMAGE_QUALITY_BEST);

			ofFile roi("adverts/" + filename+".bmp.roi",ofFile::WriteOnly);
			for(int i=0;i<4;i++){
				roi << warp.getQuad()[i].x << " " << warp.getQuad()[i].y << endl;
			}
			roi.close();
			ofNotifyEvent(finishedE,filename,this);
		}
		break;

	case QuadSelected:
		break;
	case Finished:
		break;
	}
}

void TakeAPhoto::update(){
	if(state==TakingPhoto && pixelsCopied){
		photo = photoPixels;
		pixelsCopied = false;
		updateState(UpdatedImage);
	}else if(state == Init){
		if(video->isFrameNew()){
			cout << "new frame" << endl;
			videoTex.loadData(video->getPixelsRef());
		}
	}
}

void TakeAPhoto::draw(){
	ofPushStyle();
	if(state==PhotoTaken){
		ofSetColor(255);
		photo.draw(ofGetWidth()*0.5,ofGetHeight()*0.5);
		ofSetColor(40);
		yesButton.draw();
		noButton.draw();
	}else if(state==SelectingQuad){
		ofSetColor(255);
		photo.draw(ofGetWidth()*0.5,ofGetHeight()*0.5);
		warp.draw();
		ofSetColor(40);
		yesButton.draw();
	}else{
		ofSetColor(255);
		if(video) videoTex.draw(ofGetWidth()*0.5,ofGetHeight()*0.5);
		photoButton.draw();
	}
	ofPopStyle();
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

void TakeAPhoto::newFrame(ofPixels & newFrame){
	if(state==TakingPhoto && video){
		photoPixels = video->getPixelsRef();
		pixelsCopied = true;
	}
}
