/*
 *  ofxFern.cpp
 *  
 *
 *  Created by theo on 16/12/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFern.h"

//------------------------------------------------------------------------
ofxFern::ofxFern(){
	detector = NULL;
	tracker  = NULL;
	mode = 2;
	show_tracked_locations = true;
	show_keypoints = true;
	last_frame_ok = false;
	is_initialized = false;
	last_detected.resize(8);
	last_tracked.resize(8);

	state = NotInitialized;
}

//------------------------------------------------------------------------
ofxFern::~ofxFern(){
	if( detector != NULL){
		delete detector;
		detector = NULL;
	}
	if( tracker != NULL){
		delete tracker;
		tracker = NULL;
	}
}

//------------------------------------------------------------------------
void ofxFern::setUseTexture(bool useTex){
	img.setUseTexture(useTex);
}

//------------------------------------------------------------------------
void ofxFern::draw_quadrangle(int u0, int v0, int u1, int v1, int u2, int v2, int u3, int v3){
  ofLine(ofPoint(u0, v0), ofPoint(u1, v1));
  ofLine(ofPoint(u1, v1), ofPoint(u2, v2));
  ofLine(ofPoint(u2, v2), ofPoint(u3, v3));
  ofLine(ofPoint(u3, v3), ofPoint(u0, v0));
}

//------------------------------------------------------------------------
void ofxFern::draw_detected_position(planar_pattern_detector * detector){
	draw_quadrangle(last_detected[0], last_detected[1],
			last_detected[2], last_detected[3],
			last_detected[4], last_detected[5],
			last_detected[6], last_detected[7]);
}

//------------------------------------------------------------------------
void ofxFern::draw_initial_rectangle(template_matching_based_tracker * tracker){
	draw_quadrangle( tracker->u0[0], tracker->u0[1],
		  tracker->u0[2], tracker->u0[3],
		  tracker->u0[4], tracker->u0[5],
		  tracker->u0[6], tracker->u0[7]);
}

//------------------------------------------------------------------------
void ofxFern::draw_tracked_position(template_matching_based_tracker * tracker){
	draw_quadrangle(last_tracked[0], last_tracked[1],
			last_tracked[2], last_tracked[3],
			last_tracked[4], last_tracked[5],
			last_tracked[6], last_tracked[7]);
}

//------------------------------------------------------------------------
void ofxFern::draw_tracked_locations(template_matching_based_tracker * tracker){
  for(int i = 0; i < tracker->nx * tracker->ny; i++) {
    int x1, y1;
    tracker->f.transform_point(tracker->m[2 * i], tracker->m[2 * i + 1], x1, y1);
    ofCircle(ofPoint(x1, y1), 3);
  }
}

//------------------------------------------------------------------------
void ofxFern::draw_detected_keypoints(planar_pattern_detector * detector){
  for(int i = 0; i < detector->number_of_detected_points; i++)
    ofCircle(ofPoint(detector->detected_points[i].fr_u(),
		     detector->detected_points[i].fr_v()),
	     16 * (1 << int(detector->detected_points[i].scale)));
}

//------------------------------------------------------------------------
void ofxFern::draw_recognized_keypoints(planar_pattern_detector * detector){
  for(int i = 0; i < detector->number_of_model_points; i++)
    if (detector->model_points[i].class_score > 0)
      ofCircle(ofPoint(detector->model_points[i].potential_correspondent->fr_u(),
		       detector->model_points[i].potential_correspondent->fr_v()),
	       16 * (1 << int(detector->detected_points[i].scale)));
}

//------------------------------------------------------------------------
void ofxFern::detect(IplImage * frame){
	//last_frame_ok=false;
	static bool ok = false;
	//cout << "mode: " << mode;
	if (mode == 1 || ((mode==0) && ok)) {
		//cout << " trying to track";
		ok = tracker->track(frame);
		//last_frame_ok=ok;


		if (!ok) {
			//cout << " couldn't track ";
			if (mode==0) detect(frame);
			else {
				//cout << "bailing" << endl;
				//draw_initial_rectangle(frame, tracker);
				tracker->initialize();
				last_frame_ok = false;
				state = NotDetected;
			}
		} else {
			//cout << " tracked! " << endl;
			state = Tracked;
			//draw_tracked_position(frame, tracker);
			//if (show_tracked_locations) draw_tracked_locations(frame, tracker);
		}
		//cvPutText(frame, "template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
	} else {
		//cout << " trying to detect ";
		detector->detect(frame);

		if (detector->pattern_is_detected) {
			//cout << "detected!!" <<endl;
			ok=last_frame_ok=true;
			state = Detected;

			tracker->initialize(detector->detected_u_corner[0], detector->detected_v_corner[0],
					detector->detected_u_corner[1], detector->detected_v_corner[1],
					detector->detected_u_corner[2], detector->detected_v_corner[2],
					detector->detected_u_corner[3], detector->detected_v_corner[3]);

			if (mode == 3 && tracker->track(frame)) {

				if (show_keypoints) {
					//draw_detected_keypoints(frame, detector);
					//draw_recognized_keypoints(frame, detector);
				}
				//draw_tracked_position(frame, tracker);
				//if (show_tracked_locations) draw_tracked_locations(frame, tracker);

				;//cvPutText(frame, "detection+template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
			} else {
				if (show_keypoints) {
					//draw_detected_keypoints(frame, detector);
					//draw_recognized_keypoints(frame, detector);
				}
				//draw_detected_position(frame, detector);
				;//cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
			}
		} else {
			//cout << " coudln't detect " << endl;
			ok=last_frame_ok=false;
			state = NotDetected;
			//if (show_keypoints) draw_detected_keypoints(frame, detector);

			if (mode == 3)
				;//cvPutText(frame, "detection + template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
			else
				;//cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
		}
	}

	for(int i=0;i<4; i++){
		last_detected[i*2] = detector->detected_u_corner[i];
		last_detected[i*2+1] = detector->detected_v_corner[i];
	}
	for(int i=0;i<8; i++){
		last_tracked[i] = tracker->u[i];
	}
	//cvShowImage("ferns-demo", frame);
}

//------------------------------------------------------------------------
void ofxFern::initTracker(string marker, int width, int height){
	state = Initializing;
	img.allocate(width, height); 
	marker = ofToDataPath(marker);

	int mode = 2;
	bool show_tracked_locations = true;
	bool show_keypoints = true;

	string model_image			= marker;
	string sequence_format		= "";
	string video_file			= "";

	cout << "building cache" << endl;


	affine_transformation_range range;

	//200,2500,0.0,32, 7, 4,30, 12, 5000, 100);
	//400,5000,0.0,32, 7, 4,30, 12, 10000, 200);

	/*
	model_image.c_str(), // image name
	&range, //affine_transformation_range
	200,    // maximum_number_of_points_on_model -> according to oprofile consumes lots of cpu in recognize(fine_gaussian_pyramid * pyramid, keypoint * K, float * distribution)
	1000,   // number_of_generated_images_to_find_stable_points
	0.0,	// minimum_number_of_views_rate
	32, 	// patch_size
	7, 		// yape_radius
	4,		// number_of_octaves
	10, 	// number_of_ferns
	1, 		// number_of_tests_per_fern
	10000,  // number_of_samples_for_refinement
	200);	// number_of_samples_for_test*/

	detector = planar_pattern_detector_builder::build_with_cache(

			model_image.c_str(), // image name
			&range, // affine_transformation_range
			200,    // maximum_number_of_points_on_model -> according to oprofile consumes lots of cpu in recognize(fine_gaussian_pyramid * pyramid, keypoint * K, float * distribution)
			1000,    // number_of_generated_images_to_find_stable_points
			0.0,	// minimum_number_of_views_rate
			32, 	// patch_size
			7, 		// yape_radius
			4,		// number_of_octaves
			30, 	// number_of_ferns
			6,		// number_of_tests_per_fern
			10000,   // number_of_samples_for_refinement
			200);	// number_of_samples_for_test);	// number_of_samples_for_test


	if (!detector) {
		ofLog(OF_LOG_ERROR, "Unable to build detector");
		return ;
	}

	detector->set_maximum_number_of_points_to_detect(300);



	tracker = new template_matching_based_tracker();
	string trackerfn = model_image + string(".tracker_data");

	if (!tracker->load(trackerfn.c_str())) {
		//warp the image to match the quad
		ofxCvGrayscaleImage img;
		img.setUseTexture(false);
		warped.setUseTexture(false);

		img.allocate(640,480);
		warped.allocate(640,480);
		img = detector->model_image;
		ofPoint src[4], dst[4];
		for(int i=0;i<4;i++){
			src[i].set( detector->u_corner[i], detector->v_corner[i] );
		}

		int uy,by;
		if(detector->v_corner[0] < detector->v_corner[1]){
			uy = detector->v_corner[0];
		}else{
			uy = detector->v_corner[1];
		}

		if(detector->v_corner[2] > detector->v_corner[3]){
			by = detector->v_corner[2];
		}else{
			by = detector->v_corner[3];
		}

		int lx,rx;
		if(detector->u_corner[0] < detector->u_corner[3]){
			lx = detector->u_corner[0];
		}else{
			lx = detector->u_corner[3];
		}

		if(detector->u_corner[1] > detector->u_corner[2]){
			rx = detector->u_corner[1];
		}else{
			rx = detector->u_corner[2];
		}
		dst[0].set( lx, uy );
		dst[1].set( rx, uy );
		dst[2].set( rx, by );
		dst[3].set( lx, by );

		warped.warpIntoMe(img,src,dst);


		ofLog(OF_LOG_NOTICE, "Training template matching...");
		tracker->learn(warped.getCvImage(),
			   3, // number of used matrices (coarse-to-fine)
			   40, // max motion in pixel used to train to coarser matrix
			   20, 20, // defines a grid. Each cell will have one tracked point.
			   lx,uy,
			   rx,by,
			   40, 40, // neighbordhood for local maxima selection
			   10000 // number of training samples
			   );
		tracker->save(trackerfn.c_str());
	}

	cout << "initializing tracker" << endl;
	tracker->initialize();
	is_initialized = true;

	state = NotDetected;
}

//------------------------------------------------------------------------
void ofxFern::update(ofxCvGrayscaleImage & grayImg ){
    img = grayImg;
	detect(img.getCvImage());
	img.flagImageChanged();
}

//------------------------------------------------------------------------
void ofxFern::update(ofPixels & pixels ){
    img = pixels.getPixels();
	detect(img.getCvImage());
	img.flagImageChanged();
}

//------------------------------------------------------------------------
void ofxFern::draw(float x, float y, float w, float h){
    //img.draw(x,y,w,h);
    if(!is_initialized) return;

    ofSetColor(255,0,0);
    ofPushMatrix();
    ofScale(w/img.getWidth(),h/img.getHeight());
    draw_tracked_position(tracker);
    ofPopMatrix();
    if(!last_frame_ok) return;
    ofSetColor(0,255,0);
    ofPushMatrix();
    ofScale(w/img.getWidth(),h/img.getHeight());
    draw_detected_position(detector);
    ofPopMatrix();
}

//------------------------------------------------------------------------
//just for now - will be removed
void ofxFern::setMode(int key ){

    if (key >= 0) {
		switch(char(key)) {
			case '0': mode = 0; break;
			case '1': mode = 1; break;
			case '2': mode = 2; break;
			case '3': mode = 3; break;
			case '4': show_tracked_locations = !show_tracked_locations; break;
			case '5': show_keypoints = !show_keypoints; break;
			default: ;
		}
		cout << "mode=" << mode << endl;
    }

}















