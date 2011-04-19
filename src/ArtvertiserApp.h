#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "Detector.h"
#include "TakeAPhoto.h"
#include "Comm.h"
#include "MainMenu.h"


//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file
								

class ArtvertiserApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void appFinished(const void * sender, string & filename);
		void gotAnalysis(const Artvert & artvert);

		void cameraPressed(bool & pressed);
		void artvertSelected(Artvert & artvert);

	private:
		Detector artvertiser;
		TakeAPhoto takeAPhoto;
		Comm comm;
		MainMenu menu;

		ofImage ofimg, subs_img;
		int counter;
		ofVideoGrabber grabber;
		vector<ofPoint> imgQuad;
		ofxCvColorImage colorImg, smallColorImg;
		bool allocated;

		enum State{
			Menu,
			Photo,
			Tracking
		}state;

};

#endif
