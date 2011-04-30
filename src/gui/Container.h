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
#include "ofBaseTypes.h"

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

	void addDrawable(ofPtr<ofBaseDraws> drawable);
	void removeDrawable(ofPtr<ofBaseDraws> drawable);

	int size();


	void clear();

	virtual void update();
	virtual void draw();

	virtual void enableEvents();
	virtual void disableEvents();

protected:
	virtual void repositionWidgets()=0;

	vector<ofPtr<Widget> > widgets;
	bool eventsEnabled;
};
}

#endif /* CONTAINER_H_ */
