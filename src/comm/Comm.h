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

#ifndef COMM_H_
#define COMM_H_

#include "ofxHttpUtils.h"
#include "Artvert.h"

class Comm: public ofThread {
public:
	Comm(string url="");
	virtual ~Comm();

	void setURL(string url);
	string getURL();

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
