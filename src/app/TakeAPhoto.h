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

class TakeAPhoto {
public:
	TakeAPhoto();

	void setup(ofBaseVideo & _video);
	void start();
	void stop();

	void update();
	void draw();

	void windowResized(ofResizeEventArgs & window);

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

	ofEvent<string> newPhotoE;
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

	ofBaseVideo * video;
	ofTexture videoTex;

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

	// This class cannot be copied
	const TakeAPhoto & operator=(const TakeAPhoto &){return *this;};
	TakeAPhoto(const TakeAPhoto &){};


};

#endif /* TAKEAPHOTO_H_ */
