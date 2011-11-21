/*
 * ofxGoogleMaps.cpp
 *
 *  Created on: 15/09/2011
 *      Author: arturo
 */

#include "ofxGoogleMaps.h"
#include "ofEvents.h"
#include "ofxAndroidGPS.h"

string ofxGoogleMaps::LOG_NAME = "ofxGoogleMaps";

ofxGoogleMaps::ofxGoogleMaps() {
	init = true;
	JNIEnv *env = ofGetJNIEnv();
	if (!env) {
		ofLogError(LOG_NAME) << "Failed to get the environment using GetEnv()";
		return;
	}
	jclass localClass = env->FindClass("cc/openframeworks/OFAndroidGoogleMaps");
	javaClass = (jclass) env->NewGlobalRef(localClass);
	if(!javaClass){
		ofLogError(LOG_NAME) << "Failed to get the java class for GoogleMaps";
		return;
	}


	jmethodID constructor = env->GetMethodID(javaClass,"<init>","()V");
	if(!constructor){
		ofLogError(LOG_NAME) << "Failed to get the java constructor for GoogleMaps";
		return;
	}

	javaGoogleMaps = env->NewObject(javaClass,constructor);
	if(!javaGoogleMaps){
		ofLogError(LOG_NAME) << "Failed to create java GoogleMaps";
		return;
	}
	javaGoogleMaps = env->NewGlobalRef(javaGoogleMaps);
}

ofxGoogleMaps::~ofxGoogleMaps() {

}

void ofxGoogleMaps::start(){
	ofAddListener(ofxAndroidGPS::locationChangedE,this,&ofxGoogleMaps::locationChanged);
	ofxAndroidGPS::startGPS();

	queryMap();
	queryAddress();
}

void ofxGoogleMaps::stop(){
	ofxAndroidGPS::stopGPS();
	ofRemoveListener(ofxAndroidGPS::locationChangedE,this,&ofxGoogleMaps::locationChanged);
}

void ofxGoogleMaps::setLocation(ofxLocation & location){
	JNIEnv * env = ofGetJNIEnv();
	if(!env){
		ofLogError(LOG_NAME) << "cannot show map, couldn't get jni environment";
		return;
	}

	jmethodID setLocationID = env->GetMethodID(javaClass,"setLocation","(DD)V");
	if(!setLocationID){
		ofLogError(LOG_NAME) << "cannot setLocation, couldn't get setLocation method ID";
		return;
	}

	env->CallVoidMethod(javaGoogleMaps,setLocationID,location.latitude,location.longitude);
}

string ofxGoogleMaps::getAddress(){
	return currentAddress;
}

string ofxGoogleMaps::getCountry(){
	return currentCountry;
}

string ofxGoogleMaps::getCity(){
	return currentCity;
}

ofxLocation ofxGoogleMaps::getLocation(){
	return lastLocation;
}

void ofxGoogleMaps::setZoom(int zoom){

}

int ofxGoogleMaps::getZoom(){

}

void ofxGoogleMaps::increaseZoom(){

}

void ofxGoogleMaps::decreaseZoom(){

}

double ofxGoogleMaps::distance(double lon1, double lat1, double lon2, double lat2){
	// http://www.movable-type.co.uk/scripts/latlong.html
	static double R = 6371; // earth radius in km
	return ::acos(::sin(lat1)*::sin(lat2) + ::cos(lat1)*::cos(lat2) * ::cos(lon2-lon1)) * R;
}

double ofxGoogleMaps::distance(const ofxLocation & loc1, const ofxLocation & loc2){
	return distance(loc1.longitude, loc1.latitude, loc2.longitude, loc2.latitude);
}

float ofxGoogleMaps::getHeight(){
	return height;
}

float ofxGoogleMaps::getWidth(){
	return width;
}

void ofxGoogleMaps::locationChanged(ofxLocation & _location){
	ofLogVerbose(LOG_NAME) << "location changed" << _location.latitude << "," << _location.longitude;
	lastLocation = _location;
	if(init || distance(lastLocation,prevLocation)>0.01){
		init = false;
		queryAddress();
		queryMap();
		prevLocation = lastLocation;
	}
}

void ofxGoogleMaps::queryAddress(){

	JNIEnv * env = ofGetJNIEnv();
	if(!env){
		ofLogError(LOG_NAME) << "cannot get address, couldn't get jni environment";
		return;
	}

	jboolean isCopy;

	//query address
	jmethodID methodID = env->GetMethodID(javaClass,"getAddress","(DD)Ljava/lang/String;");
	jstring jaddress = (jstring) env->CallObjectMethod(javaGoogleMaps,methodID,lastLocation.latitude,lastLocation.longitude);
	currentAddress = ofGetJNIEnv()->GetStringUTFChars(jaddress,&isCopy);

	//query country
	methodID = env->GetMethodID(javaClass,"getCountry","(DD)Ljava/lang/String;");
	jaddress = (jstring) env->CallObjectMethod(javaGoogleMaps,methodID,lastLocation.latitude,lastLocation.longitude);
	currentCountry = ofGetJNIEnv()->GetStringUTFChars(jaddress,&isCopy);

	//query city
	methodID = env->GetMethodID(javaClass,"getCity","(DD)Ljava/lang/String;");
	jaddress = (jstring) env->CallObjectMethod(javaGoogleMaps,methodID,lastLocation.latitude,lastLocation.longitude);
	currentCity = ofGetJNIEnv()->GetStringUTFChars(jaddress,&isCopy);

	ofLogVerbose(LOG_NAME) << "address changed" << currentAddress << "," << currentCountry << "," << currentCity;
}

void ofxGoogleMaps::queryMap(){
	setLocation(lastLocation);
}

void ofxGoogleMaps::showMap(int x, int y, int w, int h){
	ofLogVerbose(LOG_NAME) << "showMap";
	width = w;
	height = h;

	JNIEnv * env = ofGetJNIEnv();
	if(!env){
		ofLogError(LOG_NAME) << "cannot show map, couldn't get jni environment";
		return;
	}

	jmethodID showMapID = env->GetMethodID(javaClass,"showMap","(IIII)V");
	if(!showMapID){
		ofLogError(LOG_NAME) << "cannot show map, couldn't get showMap method ID";
		return;
	}

	env->CallVoidMethod(javaGoogleMaps,showMapID,x,y,w,h);
}

void ofxGoogleMaps::hideMap(){
	ofLogVerbose(LOG_NAME) << "hideMap";

	JNIEnv * env = ofGetJNIEnv();
	if(!env){
		ofLogError(LOG_NAME) << "cannot hide map, couldn't get jni environment";
		return;
	}


	jmethodID hideMapID = env->GetMethodID(javaClass,"hideMap","()V");
	if(!hideMapID){
		ofLogError(LOG_NAME) << "cannot hide map, couldn't get hideMap method ID";
		return;
	}

	env->CallVoidMethod(javaGoogleMaps,hideMapID);
}
