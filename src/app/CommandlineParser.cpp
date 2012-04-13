//
//  CommandlineParser.cpp
//  TheArtvertiserAndroid
//
//  Created by Damian Stewart on 12/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CommandlineParser.h"

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
	
	int c;
	while ((c = getopt (argc, argv, "b")) != -1)
	{
	   switch (c)
	   {
		   case 'b':
			   runningOnBinoculars = true;
			   break;
			   /*
		   case 'b':
			   bflag = 1;
			   break;
		   case 'c':
			   cvalue = optarg;
			   break;
		   case '?':
			   if (optopt == 'c')
				   fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			   else if (isprint (optopt))
				   fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			   else
				   fprintf (stderr,
							"Unknown option character `\\x%x'.\n",
							optopt);
			   return 1;
				*/
		   default:
			   fprintf( stderr, "Option ignored: -%c\n", optopt );
			   break;
	   }
	}
	
}