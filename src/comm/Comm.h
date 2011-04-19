/*
 * Comm.h
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#ifndef COMM_H_
#define COMM_H_

#include "ofxHttpUtils.h"

class Comm {
public:
	Comm(string url="");
	virtual ~Comm();

	void setURL(string url);
	void update();

	void sendAdvert(string filename);
	void newResponse(ofxHttpResponse & response);

	ofEvent<string> gotAnalysisE;

private:
	ofxHttpUtils httpClient;
	string url;
	string file;
	int lastTime;
	bool waitingForAnalysis,analysisReady,analysisRequested;
	bool gotTracker,gotDetector;
};

#endif /* COMM_H_ */
