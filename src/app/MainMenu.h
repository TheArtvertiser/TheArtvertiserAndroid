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
#include "VFrame.h"

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
	void downloadPressed(bool & pressed);
	void snapshotPressed(const void * sender, bool & pressed);

	void setIconCache(ofPtr<gui::IconCache> & iconCache);

	void windowResized(ofResizeEventArgs & window);

	ofEvent<bool> cameraPressedE;
	ofEvent<bool> downloadPressedE;
	ofEvent<Artvert> artvertSelectedE;

private:
	vector<Artvert> artverts;
	vector<bool>	readyCache;

	gui::CircularPB circularPB;
	ofPtr<gui::Button>		cameraButton;
	ofPtr<gui::Button>		downloadButton;
	gui::VFrame		menu;
	gui::Grid		grid;

	ofPtr<gui::IconCache> iconCache;
};

#endif /* MAINMENU_H_ */
