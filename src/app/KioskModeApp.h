/*
 * KioskModeApp.h
 *
 *  Created on: 21/11/2011
 *      Author: arturo
 */

#ifndef KIOSKMODEAPP_H_
#define KIOSKMODEAPP_H_

#include "ofEvents.h"
#include "ofFileUtils.h"
#include "Artvert.h"

class KioskModeApp {
public:
	KioskModeApp();
	virtual ~KioskModeApp();

	void setup(int secsArtvert, string artvertUID);
	void update();

	Artvert & getArtvert();
	string getCurrentArtvertName();
	int getSecsNextArtvert();

	ofEvent<ofFile> artvertChangedE;

private:
	int secsArtvert;
	int millisLastArtvert;
	int currentArtvert;
	ofDirectory artverts;
	ofFile artvertFile;

	Artvert artvert;
};

#endif /* KIOSKMODEAPP_H_ */
