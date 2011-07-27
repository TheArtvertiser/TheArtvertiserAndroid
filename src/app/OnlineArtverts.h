/*
 * OnlineArtverts.h
 *
 *  Created on: 01/05/2011
 *      Author: arturo
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
