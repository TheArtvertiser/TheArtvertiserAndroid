/*
 * IconCache.h
 *
 *  Created on: 22/04/2011
 *      Author: arturo
 */

#ifndef ICONCACHE_H_
#define ICONCACHE_H_

#include <list>
#include <map>

#include "ofImage.h"

#include "ofxThreadedImageLoader.h"

namespace gui{
class IconCache {
public:

	ofPtr<ofImage> loadImage(string filename);
	ofPtr<ofImage> loadImage(ofFile file);

	ofPtr<ofImage> getResource(string name);
	void storeResource(ofPtr<ofImage> image, string name);

private:
	list<ofPtr<ofImage> > icons;
	map<string,list<ofPtr<ofImage> >::iterator > icons_index;
	list<ofPtr<ofImage> > resources;
	map<string,list<ofPtr<ofImage> >::iterator > resources_index;
	ofxThreadedImageLoader loader;
};
}

#endif /* ICONCACHE_H_ */
