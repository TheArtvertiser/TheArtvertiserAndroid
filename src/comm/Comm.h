/*
 * Comm.h
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#ifndef COMM_H_
#define COMM_H_

#include "ofxHttpUtils.h"
#include "Artvert.h"

class Comm: public ofThread {
public:
	Comm(string url="");
	virtual ~Comm();

	void setURL(string url);

	void start();
	void stop();
	void threadedFunction();

	void sendAdvert(const Artvert & artvert);
	void downloadArtvert(Artvert & artvert);

	ofEvent<const Artvert> gotAnalysisE;

private:
	bool checkUploaded(Artvert & artvert);
	bool checkAnalized(Artvert & artvert);
	void downloadAnalisys(Artvert & artvert);
	void newResponse(ofxHttpResponse & response);
	ofxHttpResponse postAdvert(const Artvert & artvert);

	ofxHttpUtils httpClient;
	string url;
};

#endif /* COMM_H_ */
