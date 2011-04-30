/*
 * ArtvertInfo.cpp
 *
 *  Created on: 26/04/2011
 *      Author: arturo
 */

#include "ArtvertInfo.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "Button.h"

ArtvertInfo::ArtvertInfo()
:advert(new ofImage)
,refresh(false)
{
	// TODO Auto-generated constructor stub

}

ArtvertInfo::~ArtvertInfo() {
	// TODO Auto-generated destructor stub
}

void ArtvertInfo::setGeo(ofPtr<Geo> & _geo){
	geo = _geo;
}

void ArtvertInfo::show(Artvert & _artvert){
	mutex.lock();
	artvert = _artvert;


	location = artvert.getLocation();
	geo->setSize(ofGetWidth()/3.,ofGetWidth()/4.);
	geo->setLocation(location);

	ofImage cameraIcon;
	cameraIcon.loadImage("icons/camera.png");

	advert = iconCache->getResource(artvert.getCompressedImage().getAbsolutePath()+"Preview");
	if(!advert->bAllocated()){
		advert->setUseTexture(false);
		advert->loadImage(artvert.getCompressedImage());
		float ratio = advert->getHeight()/advert->getWidth();
		advert->resize(ofGetWidth()/3.,ofGetWidth()/3.*ratio);
	}

	grid.clear();
	grid.setRectangle(ofRectangle(ofGetWidth()/3.+40,50,ofGetWidth()-(ofGetWidth()/3.+60),ofGetHeight()));
	grid.setCellSize(cameraIcon.getWidth()*1.5, cameraIcon.getWidth()*1.5*3./4.);
	grid.setSpacing(20,20);
	grid.enableEvents();

	artverts = artvert.getArtverts();
	for(int i=0;i<(int)artverts.size();i++){
		grid.addWidget(ofPtr<gui::Button>(new gui::Button));
		gui::Button * button = (gui::Button*)grid.getWidget(i).get();
		ofPtr<ofImage> icon = iconCache->getResource(artverts[i].getAbsolutePath()+"Resized");
		if(!icon->bAllocated()){
			icon->setUseTexture(false);
			icon->loadImage(artverts[i]);
			float ratio = icon->getWidth()/icon->getHeight();
			float width,height;
			if(ratio>1){
				width = grid.getCellWidth();
				height = width/ratio;
			}else{
				height = grid.getCellHeight();
				width = height * ratio;
			}
			icon->resize(width,height);
		}
		button->setIcon(*icon);
		ofAddListener(button->pressedE,this,&ArtvertInfo::artvertPressed);
	}

	refresh = true;
	mutex.unlock();
}

void ArtvertInfo::stop(){
	grid.disableEvents();
	advert->setUseTexture(false);
}

void ArtvertInfo::update(){
	if(refresh){
		advert->setUseTexture(true);
		advert->reloadTexture();
		refresh = false;
	}
	grid.update();
	geo->update();
}

void ArtvertInfo::draw(){
	mutex.lock();
	advert->draw(20,20);
	geo->draw(20,ofGetHeight()-20-geo->getHeight());

	if(artvert.isReady())
		ofDrawBitmapString("Ready",advert->getWidth()+40,30);
	else
		ofDrawBitmapString("Waiting analisys",advert->getWidth()+40,30);

	ofDrawBitmapString(geo->getAddress() ,20, advert->getHeight()+40);

	grid.draw();
	mutex.unlock();
}

void ArtvertInfo::artvertPressed(const void * sender, bool & pressed){
	if(!artvert.isReady()) return;
	for(int i=0;i<grid.size();i++){
		if(grid[i].get() == sender){
			ofNotifyEvent(artvertSelectedE,artverts[i]);
			return;
		}
	}
}

Artvert & ArtvertInfo::getCurrentArtvert(){
	return artvert;
}

void ArtvertInfo::setIconCache(ofPtr<gui::IconCache> & _iconCache){
	iconCache = _iconCache;
}
