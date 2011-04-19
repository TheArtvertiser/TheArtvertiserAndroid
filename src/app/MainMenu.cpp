/*
 * MainMenu.cpp
 *
 *  Created on: 19/04/2011
 *      Author: arturo
 */

#include "MainMenu.h"

MainMenu::MainMenu()
:circularPB(15)
,doRefresh(false)
{

}

void MainMenu::setup(){
	ofImage cameraIcon;
	cameraIcon.loadImage("icons/camera.png");
	cameraButton.setIcon(cameraIcon);
	cameraButton.setRect(ofRectangle(20,20,cameraIcon.getWidth(),cameraIcon.getHeight()));
	ofAddListener(cameraButton.pressedE,this,&MainMenu::cameraPressed);
	refresh();
}

void MainMenu::enableEvents(){
	cameraButton.enableEvents();
	for(int i=0; i<(int)artverts.size(); ++i){
		snapshots[i].enableEvents();
	}
}

void MainMenu::disableEvents(){
	cameraButton.disableEvents();
	for(int i=0; i<(int)artverts.size(); ++i){
		snapshots[i].disableEvents();
	}
}

void MainMenu::refresh(){
	artverts = Artvert::listAll();
	snapshots.resize(artverts.size());
	readyCache.resize(artverts.size());
	for(int i=0; i<(int)artverts.size(); ++i){
		snapshots[i].setRect(ofRectangle(i*76+20,40+56*3/4,56,56*3/4));
		ofAddListener(snapshots[i].pressedE,this,&MainMenu::snapshotPressed);
	}
	doRefresh = true;
}

void MainMenu::update(){
	if(doRefresh){
		for(int i=0; i<(int)artverts.size(); ++i){
			ofImage img;
			img.loadImage(artverts[i].getCompressedImage());
			if(!artverts[i].isReady()) img.setImageType(OF_IMAGE_GRAYSCALE);
			img.resize(56,56*3/4);
			snapshots[i].setIcon(img);
		}
		doRefresh = false;
	}
	circularPB.update();
	for(int i=0;i<(int)artverts.size();i++){
		if(artverts[i].isReady()) readyCache[i] = true;
		else readyCache[i] = false;
	}
}

void MainMenu::draw(){
	cameraButton.draw();
	for(int i=0; i< (int)artverts.size(); i++){
		snapshots[i].draw();
		if(!readyCache[i]) circularPB.draw(i*76+20+56*0.5,40+56*3/4*1.5);
	}
}

void MainMenu::cameraPressed(bool & pressed){
	bool yes=true;
	ofNotifyEvent(cameraPressedE,yes);
}

void MainMenu::snapshotPressed(const void * sender, bool & pressed){
	cout <<  "snapshot pressed" << endl;
	for(int i=0;i<(int)snapshots.size();i++){
		if(&snapshots[i]==sender){
			ofNotifyEvent(artvertSelectedE,artverts[i]);
		}
	}
}
