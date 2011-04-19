/*
 * Artvertiser.h
 *
 *  Created on: 16/03/2011
 *      Author: arturo
 */

#ifndef ARTVERTISER_H_
#define ARTVERTISER_H_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxFern.h"

class Detector: public ofThread {
public:
	Detector();
	virtual ~Detector();

	void setup(string model, ofVideoGrabber & video, const vector<ofPoint> & srcQuad);
	void newFrame(ofPixels & pixels);

	ofMatrix4x4 getHomography();
	bool isDetected();
	bool isTracked();

	enum State{
		Training,
		Initializing,
		Running
	};

	State getState(){return state;}
	float getFps(){return fps;}
	ofxFern fern;
protected:
	void threadedFunction();

private:
	ofVideoGrabber * video;
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage img, img640;
	vector<ofPoint> srcQuad;
	string model;
	ofMatrix4x4 homography;
	ofMutex mutex;
	float fps;
	State state;
	int counter;
	bool isNewFrame;

	int fps_time;
	int frames;
	int time;
};

#endif /* ARTVERTISER_H_ */
