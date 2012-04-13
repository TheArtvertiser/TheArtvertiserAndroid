//
//  Binocular.h
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 11/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "BinocularButtons.h"
#include "BinocularMenu.h"

class Binocular
{
public:
	
	static Binocular* get();
	
	void setup( bool bDebug = false );

	class ArtvertSelectedEventInfo 
	{
	public:
		ArtvertSelectedEventInfo( Artvert& _selAdvert, ofFile& _selArtvert )
			: selectedAdvert( _selAdvert ), selectedArtvert( _selArtvert ) {};
		Artvert& selectedAdvert;
		ofFile selectedArtvert;
	};
	
	ofEvent< ArtvertSelectedEventInfo > artvertSelectedEvent;
	
private:
	
	
	
	BinocularMenu menu;
	BinocularButtons buttons;
	
};

