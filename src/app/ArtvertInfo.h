/*
 * ArtvertInfo.h
 *
 *  Created on: 26/04/2011
 *      Author: arturo
 */

#ifndef ARTVERTINFO_H_
#define ARTVERTINFO_H_

#include "Artvert.h"
#include "ofImage.h"
#include "Grid.h"
#include "IconCache.h"
#include "ofxGeoLocation.h"

class ArtvertInfo {
public:
	ArtvertInfo();
	virtual ~ArtvertInfo();

	void setGeo(ofPtr<ofxGeoLocation> & geo);

	void show(Artvert & artvert);
	void stop();

	void update();
	void draw();

	void setIconCache(ofPtr<gui::IconCache> & iconCache);

	Artvert & getCurrentArtvert();

	ofEvent<ofFile> artvertSelectedE;

private:
	void artvertPressed(const void * sender, bool & pressed);

	ofPtr<ofImage> advert;
	ofxLocation location;
	Artvert artvert;
	gui::Grid grid;
	ofPtr<gui::IconCache> iconCache;
	vector<ofFile> artverts;
	bool refresh;
	ofPtr<ofxGeoLocation> geo;
	ofMutex mutex;
	ofPolyline quad;
};

#endif /* ARTVERTINFO_H_ */
