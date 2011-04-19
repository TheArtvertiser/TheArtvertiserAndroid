/*
 * TakeAPhoto.cpp
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#include "TakeAPhoto.h"

#include "ofAppRunner.h"

TakeAPhoto::TakeAPhoto()
:video(NULL)
,state(Init)
,yesCenter(180,190,128,100)
,noCenter(340,190,100,104)
,borderButton(680,200,56,42)
,photoButton(borderButton)
,yesButton(yesCenter)
,noButton(noCenter)
,pixelsCopied(false)
{

}

void TakeAPhoto::setup(ofVideoGrabber & _video){
	video = &_video;

	quad.resize(4);
	quad[0].set(20,20);
	quad[1].set(600,20);
	quad[2].set(600,440);
	quad[3].set(20,440);

	warp.setMinDistance(ofGetWidth()*ofGetHeight()*.0005);
	warp.setInitialQuad(quad);

	ofImage photoIcon;
	photoIcon.loadImage("icons/camera.png");
	photoButton.setIcon(photoIcon);
	ofAddListener(photoButton.pressedE,this,&TakeAPhoto::takePhoto);

	ofImage yesIcon;
	yesIcon.loadImage("icons/yes.png");
	yesButton.setIcon(yesIcon);
	ofAddListener(yesButton.pressedE,this,&TakeAPhoto::yesPressed);

	ofImage noIcon;
	noIcon.loadImage("icons/no.png");
	noButton.setIcon(noIcon);
	ofAddListener(noButton.pressedE,this,&TakeAPhoto::noPressed);


	warp.disableEvents();
	yesButton.disableEvents();
	noButton.disableEvents();
	photoButton.enableEvents();

#ifdef TARGET_ANDROID
	ofxAndroidVideoGrabber * grabber = (ofxAndroidVideoGrabber*)((ofVideoGrabber*)video)->getGrabber();
	ofAddListener(grabber->newFrameE,this,&TakeAPhoto::newFrame);
#elif defined (TARGET_LINUX)
	ofGstVideoGrabber * grabber =(ofGstVideoGrabber*) ((ofVideoGrabber*)video)->getGrabber();
	ofGstVideoUtils * videoUtils = grabber->getGstVideoUtils();
	ofAddListener(videoUtils->bufferEvent,this,&TakeAPhoto::newFrame);
#endif

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
	}
}

void TakeAPhoto::draw(){
	if(state==PhotoTaken){
		photo.draw(0,0);
		yesButton.draw();
		noButton.draw();
	}else if(state==SelectingQuad){
		photo.draw(0,0);
		warp.draw();
		yesButton.draw();
	}else{
		if(video) video->draw(0,0);
		photoButton.draw();
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

void TakeAPhoto::newFrame(ofPixels & newFrame){
	if(state==TakingPhoto && video){
		photoPixels = video->getPixelsRef();
		pixelsCopied = true;
	}
}
