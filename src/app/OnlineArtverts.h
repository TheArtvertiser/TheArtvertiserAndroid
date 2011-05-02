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

class OnlineArtverts {
public:
	OnlineArtverts(string url="");
	virtual ~OnlineArtverts();

	void setURL(string url);

	void start();
	void stop();

	void update();
	void draw();

	void windowResized(ofResizeEventArgs & window);

	void urlResponse(ofHttpResponse & response);

	void countrySelected(const void * sender, bool & pressed);
	void citySelected(const void * sender, bool & pressed);
	void roadSelected(const void * sender, bool & pressed);

	bool back();

	void listCountries();
	void listCities(const string & country);
	void listRoads(const string & country, const string & city);

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
};

#endif /* ONLINEARTVERTS_H_ */
