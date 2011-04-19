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

	void setup(ofVideoGrabber & _video);
	void update();
	void draw();


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

	ofVideoGrabber * video;
	ofPixels photoPixels;
	ofImage photo;

	State state;
	gui::Warping warp;
	vector<ofPoint> quad;
	ofRectangle yesCenter,noCenter,borderButton;
	gui::Button photoButton, yesButton, noButton;


	bool pixelsCopied;

	// This class cannot be copied
	const TakeAPhoto & operator=(const TakeAPhoto &){return *this;};
	TakeAPhoto(const TakeAPhoto &){};


};

#endif /* TAKEAPHOTO_H_ */
