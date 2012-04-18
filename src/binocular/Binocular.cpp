//
//  Binocular.cpp
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 11/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Binocular.h"

static Binocular* instance = NULL;

Binocular* Binocular::get() { 
	if ( instance == NULL )
		instance = new Binocular();
	return instance;
}

void Binocular::setup( ofVideoGrabber& grabber, bool bDebug )
{
	buttons.setup( bDebug );
	
	menu.setup( buttons );
	
	recorder.setup( grabber );
	
	ofAddListener( buttons.redAndBlueButtons, this, &Binocular::redAndBlueButtonsPressed );
	

}

void Binocular::redAndBlueButtonsPressed( bool &pressed )
{
	if ( pressed )
		recorder.toggleRecording();
}

#ifdef TARGET_OSX
void Binocular::gotFrame(  ofPixels& pixels )
{
	recorder.addFrame( pixels );
}
#endif