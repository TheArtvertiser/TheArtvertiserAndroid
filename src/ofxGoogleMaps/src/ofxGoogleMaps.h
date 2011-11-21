/*
 * ofxGoogleMaps.h
 *
 *  Created on: 15/09/2011
 *      Author: arturo
 */

#ifndef OFXGOOGLEMAPS_H_
#define OFXGOOGLEMAPS_H_



#include "ofConstants.h"
#include "CircularPB.h"
#include "ofBaseTypes.h"
#include "ofxGPS.h"
#include "ofxAndroidUtils.h"

class ofxGoogleMaps {
public:
	ofxGoogleMaps();
	virtual ~ofxGoogleMaps();

	void start();
	void stop();

	void setLocation(ofxLocation & location);

	string getAddress();
	string getCountry();
	string getCity();
	ofxLocation getLocation();

	void setZoom(int zoom);
	int getZoom();
	void increaseZoom();
	void decreaseZoom();

	double distance(double long1, double lat1, double long2, double lat2);
	double distance(const ofxLocation & loc1, const ofxLocation & loc2);


	float getHeight();
	float getWidth();

	void showMap(int x, int y, int w, int h);
	void hideMap();

	static string LOG_NAME;

private:
	void locationChanged(ofxLocation & location);

	void queryAddress();
	void queryMap();

	jobject javaGoogleMaps;
	jclass javaClass;

	ofxLocation lastLocation;
	ofxLocation prevLocation;
	string currentAddress;
	string currentCity;
	string currentCountry;
	bool init;

	float width;
	float height;
};

#endif /* OFXGOOGLEMAPS_H_ */
