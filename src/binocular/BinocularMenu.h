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

class BinocularMenu 
{
public:
	
	void setup( BinocularButtons& buttons );

private:
	void redButtonPressed( bool& tf );
	void greenButtonPressed( bool& tf );
	void blueButtonPressed( bool& tf );
	void redAndBlueButtonPressed( bool& tf );
	
};

