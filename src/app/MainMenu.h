/*
 Copyright 2011 Arturo Castro <arturo@arturocastro.net>,

 Distributed under the terms of the GNU General Public License v3.

 This file is part of The Artvertiser for Android.

 The Artvertiser for Android is free software: you can redistribute
 it and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.

 The Artvertiser for Android is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with The Artvertiser for Android.  If not, see <http://www.gnu.org/licenses/>.
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
