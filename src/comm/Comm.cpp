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

}

Comm::~Comm() {
	stop();
}

void Comm::setURL(string _url){
	url = _url;
}

void Comm::start(){
	ofAddListener(httpClient.newResponseEvent,this,&Comm::newResponse);
	startThread(true,false);
}

void Comm::stop(){
	waitForThread(true);
}

bool Comm::checkUploaded(Artvert & artvert){
	ofxHttpForm form;
	form.action = url+"/checkuploaded.of";
	form.addFormField("uid",artvert.getUID());

	ofxHttpResponse response = httpClient.submitForm(form);

	if(response.status==200){
		ofLog(OF_LOG_VERBOSE,"Comm:: " + artvert.getUID() + " already uploaded ");
	}else{
		ofLog(OF_LOG_VERBOSE,"Comm:: " + artvert.getUID() + " not uploaded yet ");
	}

	return response.status == 200;
}

void Comm::threadedFunction(){
	vector<Artvert> artverts = Artvert::listAll();
	for(int i=0;i<(int)artverts.size();i++){
		if(!artverts[i].isReady() && !checkUploaded(artverts[i])){
			ofxHttpResponse response = postAdvert(artverts[i]);
			if(response.status!=200){
				ofLog(OF_LOG_ERROR,"error sending " + artverts[i].getUID() + ":  " + response.reasonForStatus);
			}
		}
	}

	while(isThreadRunning()){
		vector<Artvert> artverts = Artvert::listAll();
		for(int i=0;i<(int)artverts.size();i++){
			if(!artverts[i].isReady()){
				ofxHttpForm form;
				form.action = url+"/checkanalized.of";
				form.addFormField("uid",artverts[i].getUID());
				ofLog(OF_LOG_VERBOSE,"checking analized " + artverts[i].getUID());
				ofxHttpResponse response = httpClient.submitForm(form);
				if(response.status==200){
					ofLog(OF_LOG_VERBOSE,"downloading analysis for " + artverts[i].getUID());
					if(!artverts[i].getDetectorData().exists()){
						ofxHttpResponse resp_detector = httpClient.getUrl(url+"/"+artverts[i].getUID()+".bmp.detector_data");
						if(resp_detector.status==200){
							ofLog(OF_LOG_VERBOSE,"got detector data correctly, saving " + ofToString(resp_detector.responseBody.size()) + " bytes");
							ofFile & detectorData = artverts[i].getDetectorData();
							detectorData.changeMode(ofFile::WriteOnly,true);
							detectorData << resp_detector.responseBody;
							detectorData.changeMode(ofFile::ReadOnly,true);
						}
					}

					if(!artverts[i].getTrackerData().exists()){
						ofxHttpResponse resp_tracker = httpClient.getUrl(url+"/"+artverts[i].getUID()+".bmp.tracker_data");
						if(resp_tracker.status==200){
							ofLog(OF_LOG_VERBOSE,"got tracker data correctly, saving " + ofToString(resp_tracker.responseBody.size()) + " bytes");
							ofFile & trackerData = artverts[i].getTrackerData();
							trackerData.changeMode(ofFile::WriteOnly,true);
							trackerData << resp_tracker.responseBody;
							trackerData.changeMode(ofFile::ReadOnly,true);
						}
					}

					if(artverts[i].isReady()){
						ofNotifyEvent(gotAnalysisE,artverts[i],this);
					}
				}
			}
		}
		ofSleepMillis(10000);
	}
}

ofxHttpResponse Comm::postAdvert(const Artvert & artvert){
	ofxHttpForm form;
	form.action = url+"/postadvert.of";
	form.addFormField("uid",artvert.getUID());
	form.addFile("artvert",artvert.getCompressedImage().getAbsolutePath());
	form.addFile("roi",artvert.getROIFile().getAbsolutePath());
	form.method = OFX_HTTP_POST;

	return httpClient.submitForm(form);
}

void Comm::sendAdvert(const Artvert & artvert){
	ofxHttpForm form;
	form.action = url+"/postadvert.of";
	form.addFormField("uid",artvert.getUID());
	form.addFile("artvert",artvert.getCompressedImage().getAbsolutePath());
	form.addFile("roi",artvert.getROIFile().getAbsolutePath());
	form.method = OFX_HTTP_POST;

	httpClient.addForm(form);
	httpClient.start();
}

void Comm::newResponse(ofxHttpResponse & response){
	cout << "new response " << response.url << endl;
	if(response.status!=200){
		cerr << "error response: " << response.status << ":" << response.reasonForStatus << endl;
		cout << response.responseBody << endl;
		return;
	}else{
		cout << "correct response: " << response.status <<  + " " + response.url << endl;
		//cout << response.responseBody << endl;
		return;
	}
}
