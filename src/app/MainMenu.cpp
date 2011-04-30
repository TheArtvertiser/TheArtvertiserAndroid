/*
 * MainMenu.cpp
 *
 *  Created on: 19/04/2011
 *      Author: arturo
 */

#include "MainMenu.h"
#include "ofAppRunner.h"

MainMenu::MainMenu()
:circularPB(15)
{

}

void MainMenu::setup(){
	ofAddListener(ofEvents.windowResized,this,&MainMenu::windowResized);

	ofImage cameraIcon;
	cameraIcon.loadImage("icons/camera.png");
	cameraButton.setIcon(cameraIcon);
	cameraButton.setPosition(ofPoint(ofGetWidth()-cameraIcon.getWidth()-20,20));
	ofAddListener(cameraButton.pressedE,this,&MainMenu::cameraPressed);

	grid.setPosition(ofPoint(20, 20));
	grid.setCellSize(cameraIcon.getWidth()*1.5, cameraIcon.getWidth()*1.5*3./4.);
	grid.setSpacing(20,20);
	refresh();
}

void MainMenu::windowResized(ofResizeEventArgs & window){
	cameraButton.setPosition(ofPoint(ofGetWidth()-cameraButton.getRect().width-20,20));
}

void MainMenu::enableEvents(){
	cameraButton.enableEvents();
	grid.enableEvents();
}

void MainMenu::disableEvents(){
	grid.clear();
	cameraButton.disableEvents();
	grid.disableEvents();
}

void MainMenu::refresh(){
	artverts = Artvert::listAll();
	grid.clear();
	readyCache.resize(artverts.size());
	for(int i=0; i<(int)artverts.size(); ++i){
		ofPtr<gui::Button> button(new gui::Button);
		grid.addWidget(button);
		ofPtr<ofImage> resizedImg = iconCache->getResource(artverts[i].getCompressedImage().getAbsolutePath()+"Resized");
		if(!resizedImg->bAllocated()){
			resizedImg->setUseTexture(false);
			resizedImg->loadImage(artverts[i].getCompressedImage());
			resizedImg->resize(grid.getCellWidth(), grid.getCellHeight());
		}
		button->setFocusedIcon(*resizedImg);
		button->setPressedIcon(*resizedImg);
		if(!artverts[i].isReady()){
			ofPtr<ofImage> bwImage;
			bwImage = iconCache->getResource(artverts[i].getCompressedImage().getAbsolutePath()+"BW");
			if(!bwImage->bAllocated()){
				bwImage->setUseTexture(false);
				*bwImage = *resizedImg;
				bwImage->setImageType(OF_IMAGE_GRAYSCALE);
			}
			button->setIcon(*bwImage);
		}else{
			button->setIcon(*resizedImg);
		}
		ofAddListener(button->pressedE,this,&MainMenu::snapshotPressed);
	}
}

void MainMenu::update(){
	circularPB.update();
	grid.update();
	for(int i=0;i<(int)artverts.size();i++){
		readyCache[i] = artverts[i].isReady();
	}
}

void MainMenu::draw(){
	cameraButton.draw();
	grid.draw();
	for(int i=0; i< (int)artverts.size(); i++){
		if(!readyCache[i]){
			gui::Button * button = (gui::Button*)grid[i].get();
			circularPB.setPosition(ofPoint(button->getRect().x+button->getRect().width*0.5,button->getRect().y+button->getRect().height*0.5));
			circularPB.draw();
		}
	}
}

void MainMenu::cameraPressed(bool & pressed){
	bool yes=true;
	ofNotifyEvent(cameraPressedE,yes);
}

void MainMenu::snapshotPressed(const void * sender, bool & pressed){
	cout <<  "snapshot pressed" << endl;
	for(int i=0;i<(int)grid.size();i++){
		if(grid[i].get()==sender){
			ofNotifyEvent(artvertSelectedE,artverts[i]);
		}
	}
}

void MainMenu::setIconCache(ofPtr<gui::IconCache> & _iconCache){
	iconCache = _iconCache;
}
