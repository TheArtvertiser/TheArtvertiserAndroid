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
#include "Binocular.h"

static const float FONT_SIZE = 28.0f;
static const float MENU_TIMEOUT = 10.0f;
static const float DEBOUNCE_TIMEOUT = 0.1f;


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
	
	if ( showing && ofGetElapsedTimef() > hideTimeout )
		showing = false;
}

void BinocularMenu::draw( ofEventArgs& args )
{
	if ( showing )
	{
		
		float fontSize = FONT_SIZE;
		float hGap = 4;
		float lineHeight = fontSize+hGap;

		// offset to keep the selection on-screen
		int linesPerScreen = float(ofGetHeight())/lineHeight;
		int midLine = linesPerScreen/2;
		float yOffset = FONT_SIZE;
		if ( selectionIndex > midLine )
		{
			if ( selectionIndex < advertArtworkPairs.size()-midLine )
				yOffset -= lineHeight*(selectionIndex-midLine);
			else
				yOffset -= lineHeight*(advertArtworkPairs.size()-2*midLine);
		}
				
		//float yOffset = FONT_SIZE - max(min(advertArtworkPairs.size()-4,selectionIndex-8),0)*lineHeight;

		ofSetColor( ofColor::black );
		
		int count =0;
		for ( int i=0; i<adverts.size(); i++) 
		{
			for ( int j=0; j<artworkFiles[i].size(); j++ )
			{
				
				string description = adverts[i].getDescription( artworkFiles[i][j] );
				
				bool highlight = (count==selectionIndex);
				int shadow = (highlight?-1:0);
				ofSetColor( highlight ? ofColor::black : ofColor::white );
				font.drawString( ofToString( count+1 )+" "+description, 10+shadow, count*lineHeight + yOffset + shadow );
				ofSetColor( highlight ? ofColor::white : ofColor::black );
				font.drawString( ofToString( count+1 )+" "+description, 11+shadow, count*lineHeight + yOffset + 1 + shadow );
				
				count++;

			}
		}
		
		
	}
}


void BinocularMenu::updateArtvertList()
{
	/// fetch list of artverts from persistence engine and update descriptions and UIDs
	advertArtworkPairs.clear();
	artworkFiles.clear();
	adverts = Artvert::listAll();
	for ( int i=0; i<adverts.size(); i++ )
	{
		artworkFiles.push_back( adverts[i].getArtverts() );
		for ( int j=0; j<artworkFiles[i].size(); j++ )
		{
			advertArtworkPairs.push_back( make_pair( i, j ) );
		}
	}
}

void BinocularMenu::redButtonPressed( bool& tf )
{
	if ( tf && showing && redDebounceTimer < ofGetElapsedTimef() )
	{
		selectionIndex--;
		if ( selectionIndex<0 )
			selectionIndex = advertArtworkPairs.size()-1;
		hideTimeout = ofGetElapsedTimef() + MENU_TIMEOUT;
		redDebounceTimer = ofGetElapsedTimef() + DEBOUNCE_TIMEOUT;
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
			hideTimeout = ofGetElapsedTimef() + MENU_TIMEOUT;
		}
		else
		{
			// tell listeners that a selection was made
			if ( selectionIndex < advertArtworkPairs.size() )
			{
				int whichAdvert = advertArtworkPairs[selectionIndex].first;
				int whichArtworkFile = advertArtworkPairs[selectionIndex].second;

				Binocular::ArtvertSelectedEventInfo info( adverts[whichAdvert], artworkFiles[whichAdvert][whichArtworkFile] );
				ofNotifyEvent( Binocular::get()->artvertSelectedEvent, info );
			}
			showing = false;
		}
	}
}

void BinocularMenu::blueButtonPressed( bool& tf )
{
	if ( tf && showing && blueDebounceTimer < ofGetElapsedTimef() )
	{
		selectionIndex++;
		if ( selectionIndex>=advertArtworkPairs.size() )
			selectionIndex = 0;
		hideTimeout = ofGetElapsedTimef() + MENU_TIMEOUT;
		blueDebounceTimer = ofGetElapsedTimef() + DEBOUNCE_TIMEOUT;
	}
}

void BinocularMenu::redAndBlueButtonPressed( bool& tf )
{
	if ( tf )
		ofLog( OF_LOG_NOTICE, "red+blue" );
}


