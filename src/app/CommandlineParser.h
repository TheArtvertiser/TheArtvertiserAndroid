//
//  CommandlineParser.h
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 12/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

class CommandlineParser
{
public:
	
	static CommandlineParser* get();
	
	void parse( int argc, char** argv );
	
	
	bool isRunningOnBinoculars() { return runningOnBinoculars; }
	
private:
	
	bool runningOnBinoculars;
	
};

