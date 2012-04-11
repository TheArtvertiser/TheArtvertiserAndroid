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

void Binocular::setup( bool bDebug )
{
	buttons.setup( bDebug );
	menu.setup( buttons );
}
