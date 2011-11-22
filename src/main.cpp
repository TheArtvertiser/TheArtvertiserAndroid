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

#include "ofMain.h"
#include "ArtvertiserApp.h"

#ifdef TARGET_ANDROID
	#include "ofAppAndroidWindow.h"
#else
	#include "ofAppGlutWindow.h"
#endif



int main(){
#ifdef TARGET_ANDROID
	ofPtr<ofAppAndroidWindow> window (new ofAppAndroidWindow);
#else
	ofPtr<ofAppGlutWindow> window (new ofAppGlutWindow);
#endif
	//ofSetupOpenGL(window, 1024, 600, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(window, 800, 400, OF_FULLSCREEN);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ArtvertiserApp() );
	return 0;
}


#ifdef TARGET_ANDROID
#include <jni.h>

//========================================================================
extern "C"{
	void Java_cc_openframeworks_OFAndroid_init( JNIEnv*  env, jobject  thiz ){
		main();
	}
}
#endif
