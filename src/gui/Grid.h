/*
 * Grid.h
 *
 *  Created on: 21/04/2011
 *      Author: arturo
 */

#ifndef GRID_H_
#define GRID_H_

#include "Widget.h"
#include "ofTypes.h"
#include "Container.h"
#include "ofEvents.h"

namespace gui{
class Grid: public Container {
public:
	Grid();
	~Grid();

	void setPosition(const ofPoint & pos);
	void setRectangle(const ofRectangle & rect);

	void setCellSize(float cellW, float cellH);
	void setSpacing(float hSpacing, float vspacing);

	float getCellWidth();
	float getCellHeight();

private:
	void repositionWidgets();
	void windowResized(ofResizeEventArgs & window);
	float cellWidth, cellHeight;
	float hSpacing, vSpacing;
	ofRectangle rect;
	bool wholescreen;

};
}

#endif /* GRID_H_ */
