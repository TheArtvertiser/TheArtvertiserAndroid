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
