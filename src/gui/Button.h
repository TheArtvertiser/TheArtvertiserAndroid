/*
 * Button.h
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "ofEvents.h"
#include "ofRectangle.h"
#include "ofPoint.h"
#include "ofImage.h"

namespace gui{
class Button {
public:
	Button( const ofRectangle & rect=ofRectangle() );

	void setRect(const ofRectangle & rect);
	void setIcon(ofImage & icon);

	void enableEvents();
	void disableEvents();

	void update();
	void draw();

	void mousePressed(ofMouseEventArgs & mouse);
	void mouseDragged(ofMouseEventArgs & mouse);
	void mouseReleased(ofMouseEventArgs & mouse);
	void mouseMoved(ofMouseEventArgs & mouse);

	ofEvent<bool> pressedE;

	enum State{
		Disabled,
		Enabled,
		Focused,
		Pressed,
		OutPressed
	};

	State getState();
private:
	enum Transition{
		CursorPressed,
		CursorReleased,
		CursorOut,
		CursorIn
	};

	void updateState(Transition transition);

	ofImage icon;
	ofRectangle rect;
	State state;
};
}

#endif /* BUTTON_H_ */
