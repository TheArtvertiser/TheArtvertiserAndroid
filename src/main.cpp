#include "ofMain.h"
#include "ArtvertiserApp.h"

#ifdef TARGET_ANDROID
	#include "ofAppAndroidWindow.h"
#else
	#include "ofAppGlutWindow.h"
#endif



int main(){
	ofSetLogLevel(OF_LOG_VERBOSE);
#ifdef TARGET_ANDROID
	ofPtr<ofAppAndroidWindow> window (new ofAppAndroidWindow);
#else
	ofPtr<ofAppGlutWindow> window (new ofAppGlutWindow);
#endif
	//ofSetupOpenGL(window, 1024, 600, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(window, 800, 400, OF_WINDOW);			// <-------- setup the GL context
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
