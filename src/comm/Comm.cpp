/*
 * Comm.cpp
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#include "Comm.h"

Comm::Comm(string url)
:url(url)
{
	waitingForAnalysis = false;
	analysisReady = false;
	gotTracker = false;
	gotDetector = false;
	analysisRequested = false;
	ofAddListener(httpClient.newResponseEvent,this,&Comm::newResponse);
}

Comm::~Comm() {
}

void Comm::setURL(string _url){
	url = _url;
}

void Comm::update(){
	if(waitingForAnalysis && ofGetElapsedTimeMillis()-lastTime >= 10000){
		if(analysisReady && !analysisRequested){
			httpClient.addUrl(url+"/"+file+".bmp.detector_data");
			httpClient.addUrl(url+"/"+file+".bmp.tracker_data");
			analysisRequested = true;
		}else{
			httpClient.addUrl(url+"/"+file+".of");
		}
		lastTime = ofGetElapsedTimeMillis();
	}
}

void Comm::sendAdvert(string filename){
	ofxHttpForm form;
	form.action = url+"/postadvert.of";
	form.addFile("artvert","adverts/" + filename+".jpg");
	form.addFile("roi","adverts/" + filename+".bmp.roi");
	form.method = OFX_HTTP_POST;
	waitingForAnalysis = true;
	analysisReady = false;
	gotTracker = false;
	gotDetector = false;
	analysisRequested = false;
	file = filename;
	httpClient.addForm(form);
	httpClient.start();
	lastTime = ofGetElapsedTimeMillis();
}

void Comm::newResponse(ofxHttpResponse & response){
	cout << "new response " << response.url << endl;
	if(response.status!=200){
		cerr << "error response" << response.status << ":" << response.reasonForStatus << endl;
		cout << response.responseBody << endl;
		return;
	}

	if(response.url == "/"+file+".of"){
		analysisReady = true;
		return;
	}

	if(analysisReady){
		if(response.url.size()<response.url.size()-string(".bmp.tracker_data").size()) return;
		cout << response.url.substr(response.url.size()-string(".bmp.tracker_data").size()) << endl;
		if(response.url.substr(response.url.size()-string(".bmp.tracker_data").size())==".bmp.tracker_data"){
			cout << " got tracker data size " << response.responseBody.size() << endl;
			ofFile file("adverts/"+response.getURLFilename(),ofFile::WriteOnly,true);
			file << response.responseBody;
			gotTracker = true;
		}else{
			if(response.url.size()<response.url.size()-string(".bmp.detector_data").size()) return;
			if(response.url.substr(response.url.size()-string(".bmp.detector_data").size())==".bmp.detector_data"){
				ofFile file("adverts/"+response.getURLFilename(),ofFile::WriteOnly,true);
				file << response.responseBody;
				gotDetector = true;

			}
		}
		if(gotTracker && gotDetector && waitingForAnalysis){
			waitingForAnalysis = false;
			ofNotifyEvent(gotAnalysisE,file,this);
		}
	}
}
