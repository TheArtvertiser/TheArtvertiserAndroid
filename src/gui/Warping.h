/*
 * Warping.h
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#ifndef WARPING_H_
#define WARPING_H_

#include <vector>
#include "ofEvents.h"
#include "ofPoint.h"

namespace gui{
	class Warping {
	public:
		Warping(float minDistance=0);

		void setMinDistance(float minDistance);
		void setInitialQuad(const vector<ofPoint> & quad, const ofPoint & offset = ofPoint());
		void enableEvents();
		void disableEvents();

		void update();
		void draw();

		void mousePressed(ofMouseEventArgs & mouse);
		void mouseDragged(ofMouseEventArgs & mouse);
		void mouseReleased(ofMouseEventArgs & mouse);
		void mouseMoved(ofMouseEventArgs & mouse){}

		const vector<ofPoint> & getQuad();

	private:
		vector<ofPoint> quad;
		float minDistance;
		int selectedPoint;
	};
}

#endif /* WARPING_H_ */
