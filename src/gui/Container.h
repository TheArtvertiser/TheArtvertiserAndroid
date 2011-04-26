/*
 * Container.h
 *
 *  Created on: 21/04/2011
 *      Author: arturo
 */

#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "Widget.h"
#include "ofTypes.h"
#include "ofPoint.h"

namespace gui{
class Container {
public:
	Container();
	virtual ~Container();

	virtual void setPosition(const ofPoint & pos)=0;

	void addWidget(ofPtr<Widget> widget);
	void removeWidget(ofPtr<Widget> widget);
	ofPtr<Widget> getWidget(int i);
	ofPtr<Widget> operator[](int i);
	int size();


	void clear();

	void update();
	void draw();

	void enableEvents();
	void disableEvents();

protected:
	virtual void repositionWidgets()=0;

	vector<ofPtr<Widget> > widgets;
	bool eventsEnabled;
};
}

#endif /* CONTAINER_H_ */
