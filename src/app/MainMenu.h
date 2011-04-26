/*
 * MainMenu.h
 *
 *  Created on: 19/04/2011
 *      Author: arturo
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <vector>

#include "Artvert.h"
#include "CircularPB.h"
#include "ofImage.h"
#include "Button.h"
#include "Grid.h"
#include "IconCache.h"

class MainMenu {
public:
	MainMenu();

	void setup();
	void enableEvents();
	void disableEvents();
	void refresh();

	void update();
	void draw();

	void cameraPressed(bool & pressed);
	void snapshotPressed(const void * sender, bool & pressed);

	void windowResized(ofResizeEventArgs & window);

	ofEvent<bool> cameraPressedE;
	ofEvent<Artvert> artvertSelectedE;

private:
	vector<Artvert> artverts;
	vector<ofPtr<gui::Button> > snapshots;
	vector<bool>	readyCache;

	gui::CircularPB circularPB;
	gui::Button		cameraButton;
	gui::Grid		grid;
	bool doRefresh;

	gui::IconCache iconCache;
};

#endif /* MAINMENU_H_ */
