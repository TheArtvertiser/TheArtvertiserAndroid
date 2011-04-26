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
,doRefresh(false)
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
	cameraButton.disableEvents();
	grid.disableEvents();
}

void MainMenu::refresh(){
	artverts = Artvert::listAll();
	grid.clear();
	snapshots.clear();
	readyCache.resize(artverts.size());
	for(int i=0; i<(int)artverts.size(); ++i){
		/*snapshots[i].setRect(ofRectangle(i*76+20,40+56*3/4,56,56*3/4));*/
		snapshots.push_back(ofPtr<gui::Button>(new gui::Button));
		grid.addWidget(snapshots[i]);
		ofAddListener(snapshots[i]->pressedE,this,&MainMenu::snapshotPressed);
	}
	doRefresh = true;
}

void MainMenu::update(){
	if(doRefresh){
		for(int i=0; i<(int)artverts.size(); ++i){
			ofPtr<ofImage> img = iconCache.loadImage(artverts[i].getCompressedImage());
			ofPtr<ofImage> resizedImg = iconCache.getResource(artverts[i].getCompressedImage().getAbsolutePath()+"Resized");
			if(!resizedImg->bAllocated()){
				*resizedImg = *img;
				resizedImg->resize(grid.getCellWidth(), grid.getCellHeight());
			}
			snapshots[i]->setFocusedIcon(*resizedImg);
			snapshots[i]->setPressedIcon(*resizedImg);
			if(!artverts[i].isReady()){
				ofPtr<ofImage> bwImage;
				bwImage = iconCache.getResource(artverts[i].getCompressedImage().getAbsolutePath()+"BW");
				if(!bwImage->bAllocated()){
					*bwImage = *resizedImg;
					bwImage->setImageType(OF_IMAGE_GRAYSCALE);
				}
				snapshots[i]->setIcon(*bwImage);
			}else{
				snapshots[i]->setIcon(*resizedImg);
			}
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
		snapshots[i]->draw();
		if(!readyCache[i]){
			circularPB.setRect(snapshots[i]->getRect());
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
	for(int i=0;i<(int)snapshots.size();i++){
		if(snapshots[i].get()==sender){
			ofNotifyEvent(artvertSelectedE,artverts[i]);
		}
	}
}
