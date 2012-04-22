//
//  BinocularVideoRecorder.h
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 16/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "ofxMovieExporter.h"

class BinocularVideoRecorder
{
public:
	BinocularVideoRecorder():recording(false), width(0), height(0) {};
	
	void setup( ofVideoGrabber& grabber );
	
	void toggleRecording();
	bool isRecording() { return recording; }
	
	void addFrame( ofPixels& pixels );
	
	void draw(ofEventArgs &args);
	
private:
	void setWidthHeight( int w, int h ) { width = w; height = h; }
	
	bool recording;
	itg::ofxMovieExporter exporter;
	
	int width, height;
	double lastFrameTime;
	double framerate;
	float debounceTimer;

};

