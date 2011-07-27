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

#ifndef ONLINEARTVERTS_H_
#define ONLINEARTVERTS_H_

#include "VFrame.h"
#include "ofURLFileLoader.h"
#include "ofConstants.h"
#include "CircularPB.h"
#include "IconCache.h"
#include "Grid.h"
#include "Label.h"
#include "ofThread.h"
#include "Artvert.h"
#include "Comm.h"

class OnlineArtverts {
public:
	OnlineArtverts(string url="");
	virtual ~OnlineArtverts();

	void setURL(string url);
	void setIconCache(ofPtr<gui::IconCache> iconCache);
	void setComm(ofPtr<Comm> comm);

	void setup();

	void start();
	void stop();

	void update();
	void draw();

	void windowResized(ofResizeEventArgs & window);

	void urlResponse(ofHttpResponse & response);

	void countrySelected(const void * sender, bool & pressed);
	void citySelected(const void * sender, bool & pressed);
	void roadSelected(const void * sender, bool & pressed);
	void artvertSelected(const void * sender, bool & pressed);

	bool back();

	void listCountries();
	void listCities(const string & country);
	void listRoads(const string & country, const string & city);
	void listArtverts(const string & country, const string & city);


	ofEvent<const Artvert> downloadedE;

private:
	gui::VFrame list;
	string url;
	string currentCountry;
	string currentCity;


	enum State{
		Countries,
		Cities,
		Roads,
		Artverts
	}state;

	gui::CircularPB circularPB;
	bool loading;

	ofURLFileLoader urlLoader;

	ofPtr<gui::IconCache> iconCache;

	gui::Grid grid;

	string listTTF;
	int listTTFSize;

	gui::Label allDownloaded;

	vector<Artvert> artverts;

	ofPtr<Comm> comm;

	class DownloaderThread: public ofThread{
	public:
		DownloaderThread(OnlineArtverts * parent);
		void download(const Artvert & artvert);
		void threadedFunction();

	private:
		queue<Artvert> pendingDownloads;
		Poco::Condition condition;
		ofMutex mutex;

		OnlineArtverts * parent;
	} downloader;
};

#endif /* ONLINEARTVERTS_H_ */
