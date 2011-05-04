/*
 * TakeAPhoto.h
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#ifndef TAKEAPHOTO_H_
#define TAKEAPHOTO_H_

#include "ofVideoGrabber.h"
#include "ofImage.h"
#include "Warping.h"
#include "Button.h"
#include "VFrame.h"
#include "ofxGeoLocation.h"
#include "ExpandablePanel.h"
#include "Label.h"
#include "Artvert.h"

#ifdef TARGET_ANDROID
#include "ofxAndroidGPS.h"
#endif

class TakeAPhoto {
public:
	TakeAPhoto();

	void setGeo(ofPtr<ofxGeoLocation> & geo);

	void setup(ofVideoGrabber & _video);
	void start();
	void stop();

	void update();
	void draw();

	void windowResized(ofResizeEventArgs & window);

	void touchDoubleTap(ofTouchEventArgs & touch);

	enum State{
		Init,
		TakingPhoto,
		PhotoTaken,
		SelectingQuad,
	};

	State getState();
	ofImage & getPhoto();

	void takePhoto(bool & pressed);
	void yesPressed(bool & pressed);
	void noPressed(bool & pressed);
	void exitPressed(bool & pressed);

	void newFrame(ofPixels & newFrame);

	ofEvent<Artvert> newPhotoE;
	ofEvent<bool> exitE;

private:

	enum Transition{
		Start,
		PhotoPressed,
		YesPressed,
		NoPressed,
		UpdatedImage,
		Stop
	};

	void updateState(Transition transition);
	void initWarp();

	ofBaseVideoDraws * video;

	ofPixels photoPixels;
	ofImage photo;

	State state;
	gui::Warping warp;
	vector<ofPoint> quad;
	ofPtr<gui::Button> cameraButton, yesButton, noButton, exitButton;
	gui::VFrame borderFrame;
	ofImage photoIcon;
	ofImage yesIcon;
	ofImage noIcon;
	ofImage exitIcon;

	float scale;
	float videoWidth,videoHeight;

	bool pixelsCopied;

	ofPoint originalVideoAnchor;

	ofPtr<ofxGeoLocation> geo;

	ofPtr<gui::Label> address;
	gui::ExpandablePanel geoPanel;

	// This class cannot be copied
	const TakeAPhoto & operator=(const TakeAPhoto &){return *this;};
	TakeAPhoto(const TakeAPhoto &){};


};

#endif /* TAKEAPHOTO_H_ */
