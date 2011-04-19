/*
 *  ofxFern.h
 *  
 *
 *  Created by theo on 16/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "ofxOpenCv.h"

#include "mcv.h"
#include "planar_pattern_detector_builder.h"
#include "template_matching_based_tracker.h"

class ofxFern{
	public:
		
		ofxFern();
		~ofxFern();

		void initTracker(string marker, int width, int height);
		void update(ofxCvGrayscaleImage & grayImg );
		void update(ofPixels & pixels );

		void setUseTexture(bool useTex);

		void draw(float x, float y, float w, float h);
		
		//just for now - will be removed
		void setMode(int key);
		
		bool isInitialized(){
			return is_initialized;
		}

		vector<ofPoint> getLastQuad(){
			vector<ofPoint> lastQuad(4);
			if(state==Tracked){
				for(int i=0;i<4;i++){
					lastQuad[i].set(last_tracked[i*2],last_tracked[i*2+1]);
				}
			}else if(state==Detected){
				for(int i=0;i<4;i++){
					lastQuad[i].set(last_detected[i*2],last_detected[i*2+1]);
				}
			}
			return lastQuad;
		}

		enum State{
			NotInitialized,
			Initializing,
			NotDetected,
			Detected,
			Tracked
		} state;
		ofxCvGrayscaleImage warped;
	protected:
				
		void draw_quadrangle(int u0, int v0, int u1, int v1, int u2, int v2, int u3, int v3);
		void draw_detected_position(planar_pattern_detector * detector);
		void draw_initial_rectangle(template_matching_based_tracker * tracker);
		void draw_tracked_position(template_matching_based_tracker * tracker);
		void draw_tracked_locations(template_matching_based_tracker * tracker);
		void draw_detected_keypoints(planar_pattern_detector * detector);
		void draw_recognized_keypoints(planar_pattern_detector * detector);

		void detect(IplImage * frame);

		
		ofxCvGrayscaleImage img;

		int mode;
		bool show_tracked_locations;
		bool show_keypoints;
		planar_pattern_detector * detector;
		template_matching_based_tracker * tracker;
		bool last_frame_ok;

		bool is_initialized;
		
		vector<int> last_detected;
		vector<int> last_tracked;

};
