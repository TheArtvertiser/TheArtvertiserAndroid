//
//  CommandlineParser.cpp
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 12/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CommandlineParser.h"
#include "ofMain.h"

CommandlineParser* CommandlineParser::get()
{
	static CommandlineParser* instance = NULL;
	if ( instance == NULL )
		instance = new CommandlineParser();
	return instance;
}



void CommandlineParser::parse( int argc, char **argv )
{
	runningOnBinoculars = false;
	

	for ( int i=0; i<argc; i++ )
	{
		if ( string(argv[i]) == "-b" )
		   runningOnBinoculars = true;
		else
			ofLog( OF_LOG_WARNING, "Option ignored: %s\n", argv[i] );
	}
	
}