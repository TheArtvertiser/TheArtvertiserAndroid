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

	ofEvent<bool> cameraPressedE;
	ofEvent<Artvert> artvertSelectedE;

private:
	vector<Artvert> artverts;
	vector<gui::Button> snapshots;
	vector<bool>	readyCache;

	gui::CircularPB circularPB;
	gui::Button		cameraButton;
	bool doRefresh;
};

#endif /* MAINMENU_H_ */
