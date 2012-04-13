//
//  BinocularMenu.h
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 11/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "BinocularButtons.h"
#include "IconCache.h"
#include "Artvert.h"
#include "ArtvertInfo.h"

class BinocularMenu 
{
public:
	
	void setup( BinocularButtons& buttons );

	ofEvent<ofFile> artvertSelected;
	
private:	
	void update( ofEventArgs& args );
	void draw( ofEventArgs& args );
	
	void updateArtvertList();

	void redButtonPressed( bool& tf );
	void greenButtonPressed( bool& tf );
	void blueButtonPressed( bool& tf );
	void redAndBlueButtonPressed( bool& tf );
	
	
	
	
	bool showing;
	int selectionIndex;
	vector<pair<int, int> > advertArtworkPairs;
	
	vector<Artvert> adverts;
	vector<vector<ofFile > > artworkFiles;
	
	
	ofTrueTypeFont font;

	gui::IconCache iconCache;
	
};

