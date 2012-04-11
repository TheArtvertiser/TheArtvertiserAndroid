//
//  BinocularButtons.h
//  TheArtvertiserServer
//
//  Created by Damian Stewart on 11/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

class BinocularButtons
{
public:
	BinocularButtons(): buttonState( "000" ), buttonStateChanged(false), currentRawButtonState( "000" )
	{};
	~BinocularButtons() { shutdown(); }
	
	void setup( bool bDebug = false );
	void shutdown();
	

	
	ofEvent<bool> redButton;
	ofEvent<bool> greenButton;
	ofEvent<bool> blueButton;
	ofEvent<bool> redAndBlueButtons; // when both red and blue are pressed. only fires on press, not release
	
private:
	
	void update( ofEventArgs& args );
	
	string buttonState; // a 3-char string with '0' = off and '1' = on for each button
	
	bool buttonStateChanged; 
	string currentRawButtonState;

	ofSerial serialConnection;
	
		
};

