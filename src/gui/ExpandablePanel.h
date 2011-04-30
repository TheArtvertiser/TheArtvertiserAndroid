/*
 * ExpandablePanel.h
 *
 *  Created on: 29/04/2011
 *      Author: arturo
 */

#ifndef EXPANDABLEPANEL_H_
#define EXPANDABLEPANEL_H_

#include "ofEvents.h"
#include "VFrame.h"

namespace gui{

class ExpandablePanel: public VFrame {
public:
	ExpandablePanel();
	virtual ~ExpandablePanel();

	void setRectCompressed(const ofRectangle & rect);
	void setRectExpanded(const ofRectangle & rect);

	void expand();
	void compress();
	void toggleExpand();

	void update();
	void draw();

	void mousePressed(ofMouseEventArgs & mouse);
	void mouseDragged(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void mouseMoved(ofMouseEventArgs & mouse);

	void enableEvents();
	void disableEvents();

	ofEvent<bool> expandedE;

private:
	void setRect(const ofRectangle & rect);
	ofRectangle rectCompressed;
	ofRectangle rectExpanded;
	ofRectangle rect;
	bool isExpanded;
};

}

#endif /* EXPANDABLEPANEL_H_ */
