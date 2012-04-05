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

#include "TakeAPhoto.h"

#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "Artvert.h"

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

void TakeAPhoto::setGeo(ofPtr<ofxGeoLocation> & _geo){
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


	ofAddListener(ofEvents().windowResized,this,&TakeAPhoto::windowResized);

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
	if(!video || video->getWidth()==0 || video->getHeight()==0) return;
	initWarp();

	int borderX = window.width*0.5 + videoWidth*0.5 + yesIcon.getWidth()*0.1;
	int borderY = yesIcon.getWidth()*0.1;
	borderFrame.setPosition(ofPoint(borderX,borderY));
}


void TakeAPhoto::updateState(Transition transition){
	if(transition==Stop){
		ofLogVerbose("TakeAPhoto","stopping");
		video->resetAnchor();
		borderFrame.disableEvents();
		state = Init;
		geo->stop();
		ofRemoveListener(ofEvents().touchDoubleTap,this,&TakeAPhoto::touchDoubleTap);
		bool yes;
		ofNotifyEvent(exitE,yes,this);
		return;
	}

	switch(state){
	case Init:
		if(transition==Start){
			ofLogVerbose("TakeAPhoto","starting");
			borderFrame.clear();
			borderFrame.addWidget(cameraButton);
			borderFrame.addWidget(exitButton);
			borderFrame.enableEvents();
			video->setAnchorPercent(0.5,0.5);
			state = TakingPhoto;
			geo->setSize(320,240);
			geo->start();
			geoPanel.compress();
			ofAddListener(ofEvents().touchDoubleTap,this,&TakeAPhoto::touchDoubleTap);
		}

	case TakingPhoto:
		if(transition==PhotoPressed){
			ofLogVerbose("TakeAPhoto","photo pressed");
			borderFrame.clear();
			state = PhotoTaken;
		}
		break;

	case PhotoTaken:
		if(transition==UpdatedImage){
			ofLogVerbose("TakeAPhoto","photo stored in memory");
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

			ofLogVerbose("TakeAPhoto","advert complete " + filename);
			Artvert advert(filename,"adverts/");

			photo.saveImage(advert.getCompressedImage(),OF_IMAGE_QUALITY_BEST);
			photo.saveImage(advert.getModel(),OF_IMAGE_QUALITY_BEST);

			ofLogVerbose("TakeAPhoto","image saved");

			ofFile roi = advert.getROIFile();
			roi.changeMode(ofFile::WriteOnly);
			for(int i=0;i<4;i++){
				roi << (int)warp.getQuad()[i].x << " " << (int)warp.getQuad()[i].y << endl;
			}
			roi.close();

			ofLogVerbose("TakeAPhoto","roi saved");

			ofFile location = advert.getLocationFile();
			location.changeMode(ofFile::WriteOnly);
			ofxLocation loc = geo->getLocation();
			location << loc;
			location.close();

			ofLogVerbose("TakeAPhoto") << "location stored: " << loc;

			ofFile md5 = advert.getMD5File();
			md5.changeMode(ofFile::WriteOnly);
			string md5sig = advert.generateMD5();
			md5 << md5sig;
			md5.close();

			ofLogVerbose("TakeAPhoto") << "md5 stored: " << md5sig;



			ofNotifyEvent(newPhotoE,advert,this);
		}
		if(transition==NoPressed){
			ofLogVerbose("TakeAPhoto","photo cancelled");
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
	
#ifdef TARGET_OSX
	if ( ((ofVideoGrabber*)video)->isFrameNew() )
	{
		newFrame( video->getPixelsRef() );
	}
#endif
	

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
	ofLog(OF_LOG_NOTICE, "newFrame" );
	if(state==TakingPhoto && video){
		ofLog(OF_LOG_NOTICE, "newFrame storing pixels");
		photoPixels = video->getPixelsRef();
		pixelsCopied = true;
	}
}

void TakeAPhoto::touchDoubleTap(ofTouchEventArgs & touch){
	geo->increaseZoom();
}
