/*
 * IconCache.cpp
 *
 *  Created on: 22/04/2011
 *      Author: arturo
 */

#include "IconCache.h"

namespace gui{
ofPtr<ofImage> IconCache::loadImage(string filename){
	if(icons_index.find(filename)==icons_index.end()){
		icons.push_back(ofPtr<ofImage>(new ofImage()));
		icons_index[filename] = icons.end();
		icons_index[filename]--;
		icons_index[filename]->get()->loadImage(filename);
		//loader.loadFromDisk((*icons_index[filename]).get(),filename);
	}

	return *icons_index[filename];
}

ofPtr<ofImage> IconCache::loadImage(ofFile file){
	return loadImage(file.getAbsolutePath());
}

ofPtr<ofImage> IconCache::getResource(string filename){
	if(icons_index.find(filename)==icons_index.end()){
		icons.push_back(ofPtr<ofImage>(new ofImage()));
		icons_index[filename] = icons.end();
		icons_index[filename]--;
		//icons_index[filename]->get()->loadImage(filename);
		//loader.loadFromDisk((*icons_index[filename]).get(),filename);
	}

	return *icons_index[filename];
}

void IconCache::storeResource(ofPtr<ofImage> image,string filename){
	if(icons_index.find(filename)!=icons_index.end()){
		icons.erase(icons_index[filename]);
	}
	icons.push_back(image);
	icons_index[filename] = icons.end();
	icons_index[filename]--;
}
}
