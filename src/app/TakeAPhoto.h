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

class TakeAPhoto {
public:
	TakeAPhoto();

	void setup(ofBaseVideo & _video);
	void update();
	void draw();

	void windowResized(ofResizeEventArgs & window);

	enum State{
		Init,
		TakingPhoto,
		PhotoTaken,
		SelectingQuad,
		QuadSelected,
		Finished
	};

	State getState();
	ofImage & getPhoto();

	void takePhoto(bool & pressed);
	void yesPressed(bool & pressed);
	void noPressed(bool & pressed);

	void newFrame(ofPixels & newFrame);

	ofEvent<string> finishedE;

private:

	enum Transition{
		PhotoPressed,
		YesPressed,
		NoPressed,
		UpdatedImage
	};

	void updateState(Transition transition);

	ofBaseVideo * video;
	ofTexture videoTex;

	ofPixels photoPixels;
	ofImage photo;

	State state;
	gui::Warping warp;
	vector<ofPoint> quad;
	ofRectangle yesCenter,noCenter,borderButton;
	gui::Button photoButton, yesButton, noButton;
	ofImage photoIcon;
	ofImage yesIcon;
	ofImage noIcon;


	bool pixelsCopied;

	// This class cannot be copied
	const TakeAPhoto & operator=(const TakeAPhoto &){return *this;};
	TakeAPhoto(const TakeAPhoto &){};


};

#endif /* TAKEAPHOTO_H_ */
