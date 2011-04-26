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

	void setCellSize(float cellW, float cellH);
	void setSpacing(float hSpacing, float vspacing);

	float getCellWidth();
	float getCellHeight();

private:
	void repositionWidgets();
	void windowResized(ofResizeEventArgs & window);
	ofPoint pos;
	float cellWidth, cellHeight;
	float hSpacing, vSpacing;


};
}

#endif /* GRID_H_ */
