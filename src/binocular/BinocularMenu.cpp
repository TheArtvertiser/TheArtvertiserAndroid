//
//  BinocularMenu.cpp
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 11/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "BinocularMenu.h"
#include "BinocularButtons.h"
#include "ofMain.h"

void BinocularMenu::setup( BinocularButtons& buttons )
{
	ofAddListener( buttons.redButton, this, &BinocularMenu::redButtonPressed );
	ofAddListener( buttons.blueButton, this, &BinocularMenu::blueButtonPressed );
	ofAddListener( buttons.greenButton, this, &BinocularMenu::greenButtonPressed );
	ofAddListener( buttons.redAndBlueButtons, this, &BinocularMenu::redAndBlueButtonPressed );
}

void BinocularMenu::redButtonPressed( bool& tf )
{
	if ( tf )
		ofLog( OF_LOG_NOTICE, "red" );
}
void BinocularMenu::greenButtonPressed( bool& tf )
{
	if ( tf )
		ofLog( OF_LOG_NOTICE, "green" );
}
void BinocularMenu::blueButtonPressed( bool& tf )
{
	if ( tf )
		ofLog( OF_LOG_NOTICE, "blue" );
}
void BinocularMenu::redAndBlueButtonPressed( bool& tf )
{
	if ( tf )
		ofLog( OF_LOG_NOTICE, "red+blue" );
}


