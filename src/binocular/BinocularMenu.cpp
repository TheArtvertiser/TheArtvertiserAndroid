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
#include "Artvert.h"

static const float FONT_SIZE = 32.0f;


void BinocularMenu::setup( BinocularButtons& buttons )
{
	ofAddListener( buttons.redButton, this, &BinocularMenu::redButtonPressed );
	ofAddListener( buttons.blueButton, this, &BinocularMenu::blueButtonPressed );
	ofAddListener( buttons.greenButton, this, &BinocularMenu::greenButtonPressed );
	ofAddListener( buttons.redAndBlueButtons, this, &BinocularMenu::redAndBlueButtonPressed );
	
	ofAddListener( ofEvents().update, this, &BinocularMenu::update );
	ofAddListener( ofEvents().draw, this, &BinocularMenu::draw );
	
	
	font.loadFont( "fonts/Delicious-Roman.otf", FONT_SIZE );
	
	selectionIndex = 0;
	
}

void BinocularMenu::update( ofEventArgs& args )
{
	
}

void BinocularMenu::draw( ofEventArgs& args )
{
	if ( showing )
	{
		
		float fontSize = FONT_SIZE;
		float hGap = 4;
		float lineHeight = fontSize+hGap;

		// offset to keey the selection on-screen
		float yOffset = FONT_SIZE + max(selectionIndex-5,0)*lineHeight;

		ofSetColor( ofColor::black );
		
		for ( int i=0; i<artverts.size(); i++) 
		{
			string description = artverts[i].getDescription();
			
			ofSetColor( ofColor::white );
			font.drawString( ofToString( i+1 )+description, 10, i*lineHeight + yOffset );
			ofSetColor( ofColor::black );
			font.drawString( ofToString( i+1 )+description, 11, i*lineHeight + yOffset + 1 );
			if ( i == selectionIndex )
			{
				ofSetColor( ofColor::white );
				font.drawString( ofToString( i+1 )+description, 12, i*lineHeight + yOffset + 2 );
			}
		}
		
		
	}
}


void BinocularMenu::updateArtvertList()
{
	/// fetch list of artverts from persistence engine and update descriptions and UIDs
	artverts = Artvert::listAll();
}

void BinocularMenu::redButtonPressed( bool& tf )
{
	if ( tf && showing )
	{
		selectionIndex--;
		if ( selectionIndex<0 )
			selectionIndex = artverts.size()-1;
	}
}

void BinocularMenu::greenButtonPressed( bool& tf )
{
	if ( tf )
	{
		if ( !showing )
		{
			updateArtvertList();
			showing = true;
		}
		else
		{
			// tell listeners that a selection was made
			if ( selectionIndex < artverts.size() )
			{
				string selectedUID = artverts[selectionIndex].getUID();
				ofLog( OF_LOG_NOTICE, "ofNotifyEvent( artvertSelected, artverts[selectionIndex]. " );
			}
			showing = false;
		}
	}
}

void BinocularMenu::blueButtonPressed( bool& tf )
{
	if ( tf && showing )
	{
		selectionIndex++;
		if ( selectionIndex>=artverts.size() )
			selectionIndex = 0;
	}
}

void BinocularMenu::redAndBlueButtonPressed( bool& tf )
{
	if ( tf )
		ofLog( OF_LOG_NOTICE, "red+blue" );
}


