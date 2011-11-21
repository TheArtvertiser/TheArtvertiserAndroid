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

#include "KioskModeApp.h"
#include "ofUtils.h"

KioskModeApp::KioskModeApp() {

}

KioskModeApp::~KioskModeApp() {
}


void KioskModeApp::setup(int _secsArtvert, string artvertUID){
	secsArtvert = _secsArtvert;
	millisLastArtvert = ofGetElapsedTimeMillis();
	artverts.listDir("artverts");
	currentArtvert = 0;
	artvert = Artvert(artvertUID);
	if(artverts.size()){
		artvertFile = artverts.getFile(currentArtvert);
		ofNotifyEvent(artvertChangedE,artvertFile);
	}

}

void KioskModeApp::update(){
	if(artverts.size() && ofGetElapsedTimeMillis()-millisLastArtvert>=secsArtvert*1000){
		currentArtvert++;
		currentArtvert %= artverts.size();
		millisLastArtvert = ofGetElapsedTimeMillis();
		artvertFile = artverts.getFile(currentArtvert);
		ofNotifyEvent(artvertChangedE,artvertFile);
	}
}


Artvert & KioskModeApp::getArtvert(){
	return artvert;
}


string KioskModeApp::getCurrentArtvertName(){
	return artvertFile.getFileName();
}

int KioskModeApp::getSecsNextArtvert(){
	return float((secsArtvert*1000) - (ofGetElapsedTimeMillis() - millisLastArtvert))/1000.f;
}
