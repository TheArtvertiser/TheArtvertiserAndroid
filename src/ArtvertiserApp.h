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

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "Detector.h"
#include "TakeAPhoto.h"
#include "Comm.h"
#include "MainMenu.h"
#include "ArtvertInfo.h"
#include "IconCache.h"
#include "ofxGoogleMaps.h"
#include "OnlineArtverts.h"
#include "OFActivityJNI.h"
#include "KioskModeApp.h"

#ifdef TARGET_ANDROID
#include "ofxAvahiCoreBrowser.h"
#else
#include "ofxAvahiClientBrowser.h"
#endif

								
#ifdef TARGET_ANDROID
	#include "ofxAndroid.h"
	class ArtvertiserApp : public ofxAndroidApp{
#else
	class ArtvertiserApp : public ofBaseApp{
#endif

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

		void appFinished(const void * sender, bool & finished);
		void newPhoto(const void * sender, Artvert & filename);
		void gotAnalysis(const Artvert & artvert);

		void cameraPressed(bool & pressed);
		void downloadPressed(bool & pressed);
		void advertSelected(Artvert & artvert);
		void artvertSelected(ofFile & artvert);

		void avahiServiceFound(ofxAvahiService & service);

		bool backPressed();


	private:
		Detector artvertiser;
		TakeAPhoto takeAPhoto;
		ofPtr<Comm> comm;
		MainMenu menu;
		ArtvertInfo artvertInfo;
		OnlineArtverts onlineArtverts;
		KioskModeApp kioskMode;

		ofImage ofimg, subs_img;
		int counter;
		ofVideoGrabber grabber;
		vector<ofPoint> imgQuad;
		ofxCvColorImage colorImg, smallColorImg;
		gui::CircularPB circularPB;

		ofPtr<gui::IconCache> iconCache;
		ofPtr<ofxGoogleMaps> geo;

		bool allocated;

		bool refreshArtvert;
		bool refreshMenu;

		int secsArtvert;
		int millisLastArtvert;

		enum State{
			Menu,
			OnlineList,
			Photo,
			Info,
			Tracking,
			Kiosk
		}state;

#ifdef TARGET_ANDROID
		ofxAvahiCoreBrowser avahi;
#else
		ofxAvahiClientBrowser avahi;
#endif

		ofxXmlSettings settings;

		bool showDebugInfo;

		int camW;
		int camH;
};

#endif
