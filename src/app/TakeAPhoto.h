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
#include "OFActivityJNI.h"

#ifdef TARGET_ANDROID
#include "ofxAndroidGPS.h"
#include "ofxGoogleMaps.h"
#endif

class TakeAPhoto {
public:
	TakeAPhoto();

	void setGeo(ofPtr<ofxGoogleMaps> & geo);

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

	ofPtr<ofxGoogleMaps> geo;

	ofPtr<gui::Label> address;
	//gui::ExpandablePanel geoPanel;

	// This class cannot be copied
	const TakeAPhoto & operator=(const TakeAPhoto &){return *this;};
	TakeAPhoto(const TakeAPhoto &){};


};

#endif /* TAKEAPHOTO_H_ */
