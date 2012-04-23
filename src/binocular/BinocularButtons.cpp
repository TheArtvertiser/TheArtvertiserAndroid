//
//  BinocularButtons.cpp
//  TheArtvertiserServer
//
//  Created by Damian Stewart on 11/04/12.
//  Copyright (c) 2012 Damian Stewart. All rights reserved.
//

#include "BinocularButtons.h"
#include "ofMain.h"

void BinocularButtons::setup( bool bDebug )
{
	
	ofAddListener(ofEvents().update, this, &BinocularButtons::update);  
	
}

void BinocularButtons::tryToConnect()
{
	// setup the first device for 9600
#ifdef TARGET_OSX
	serialConnection.listDevices();
	isConnected = serialConnection.setup( 0, 9600 );
#else
	isConnected = serialConnection.setup( 0, 9600 );
//	serialConnection.setup("/dev/ttyUSB0", 9600);
#endif
}

void BinocularButtons::shutdown()
{
	ofRemoveListener(ofEvents().update, this, &BinocularButtons::update);  
}

void BinocularButtons::update( ofEventArgs& args )
{
	// read button state
	int readCount =0;
	if ( !isConnected )
		tryToConnect();
	if ( isConnected && serialConnection.available() >= 4 )
	{
		char bytesRead[5] = { 0, 0, 0, 0, 0 }; // 0|1 0|1 0|1 \n <null>
		for ( int i=0; i<4; i++ )
		{
			int read = serialConnection.readByte();
			if ( read < 0 )
			{
				ofLog( OF_LOG_ERROR, "buttons: error %i reading serial", read);
				break;
			}
			else
			{
				bytesRead[i] = read;
				readCount++;
				if ( bytesRead[i] == '\n' )
					break;
			}
		}
		if ( readCount < 4 || bytesRead[3] != '\n' )
		{
			ofLog( OF_LOG_ERROR, "buttons: malformed data from serial: %i %i %i %i\n", bytesRead[0], bytesRead[1], bytesRead[2], bytesRead[3] );
		}
		else
		{
			/*char buttonState[4];
			for ( int i=0; i<3; i++ )
				buttonState[i] = bytesRead[i];
			buttonState[3] = 0;*/
			char* buttonState = bytesRead;
			if ( buttonState != currentRawButtonState )
			{
				// different: figure out what changed
				bool alsoSendBlueAndRedEvent = false;
				for ( int i=0; i<3; i++ )
				{
					if ( bytesRead[i] == currentRawButtonState[i] )
						continue;
					if ( (bytesRead[i] == '1' && currentRawButtonState[i] == '0') ||
						( bytesRead[i] == '0' && currentRawButtonState[i] == '1' ) )
					{
						// i was pressed or released
						bool newState = ( bytesRead[i]=='1' );
						if ( i== 0 )
							ofNotifyEvent( redButton, newState );
						else if ( i == 1 )
							ofNotifyEvent( greenButton, newState );
						else if ( i== 2 )
							ofNotifyEvent( blueButton, newState );

						
					}
					if ( bytesRead[i] == '1' && currentRawButtonState[i] == '0' )
					{
						// special: catch blue+red, 
						if ( ( i==2 && bytesRead[0] == '1' ) || ( i==0 && bytesRead[2] == '1' ) )
						{
							alsoSendBlueAndRedEvent = true;
						}
					}
				}
				
				// now also send the caught blue+red down arguments
				if ( alsoSendBlueAndRedEvent )
				{
					bool active = true;
					ofNotifyEvent( redAndBlueButtons, active );
				}
				
				currentRawButtonState = buttonState;
				
			}
		}
		
	}
}
