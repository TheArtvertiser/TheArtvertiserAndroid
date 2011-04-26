/*
 * Comm.cpp
 *
 *  Created on: 18/04/2011
 *      Author: arturo
 */

#include "Comm.h"
#include "PersistanceEngine.h"

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

bool Comm::checkAnalized(Artvert & artvert){
	ofxHttpForm form;
	form.action = url+"/checkanalized.of";
	form.addFormField("uid",artvert.getUID());
	ofLog(OF_LOG_VERBOSE,"checking analized " + artvert.getUID());
	ofxHttpResponse response = httpClient.submitForm(form);
	return response.status==200;
}

void Comm::downloadArtvert(Artvert & artvert){
	if(!artvert.getCompressedImage().exists()){
		ofxHttpResponse resp_compressed = httpClient.getUrl(url+"/"+artvert.getUID()+".jpg");
		if(resp_compressed.status==200){
			ofLog(OF_LOG_VERBOSE,"got jpg correctly, saving " + ofToString(resp_compressed.responseBody.size()) + " bytes");
			ofFile & compressed = artvert.getCompressedImage();
			compressed.changeMode(ofFile::WriteOnly,true);
			compressed << resp_compressed.responseBody;
			compressed.changeMode(ofFile::ReadOnly,true);

			ofImage model;
			model.setUseTexture(false);
			model.loadImage(compressed.getAbsolutePath());
			model.saveImage(artvert.getModel());
		}
	}

	if(!artvert.getROIFile().exists()){
		ofxHttpResponse resp_roi = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.roi");
		if(resp_roi.status==200){
			ofLog(OF_LOG_VERBOSE,"got roi correctly, saving " + ofToString(resp_roi.responseBody.size()) + " bytes");
			ofFile & roi = artvert.getROIFile();
			roi.changeMode(ofFile::WriteOnly,true);
			roi << resp_roi.responseBody;
			roi.changeMode(ofFile::ReadOnly,true);
		}
	}

	if(!artvert.getDetectorData().exists()){
		ofxHttpResponse resp_detector = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.detector_data");
		if(resp_detector.status==200){
			ofLog(OF_LOG_VERBOSE,"got detector data correctly, saving " + ofToString(resp_detector.responseBody.size()) + " bytes");
			ofFile & detectorData = artvert.getDetectorData();
			detectorData.changeMode(ofFile::WriteOnly,true);
			detectorData << resp_detector.responseBody;
			detectorData.changeMode(ofFile::ReadOnly,true);
		}
	}

	if(!artvert.getTrackerData().exists()){
		ofxHttpResponse resp_tracker = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.tracker_data");
		if(resp_tracker.status==200){
			ofLog(OF_LOG_VERBOSE,"got tracker data correctly, saving " + ofToString(resp_tracker.responseBody.size()) + " bytes");
			ofFile & trackerData = artvert.getTrackerData();
			trackerData.changeMode(ofFile::WriteOnly,true);
			trackerData << resp_tracker.responseBody;
			trackerData.changeMode(ofFile::ReadOnly,true);
		}
	}

}

void Comm::downloadAnalisys(Artvert & artvert){
	ofLog(OF_LOG_VERBOSE,"downloading analysis for " + artvert.getUID());
	if(!artvert.getDetectorData().exists()){
		if(artvert.getAliasUID()!=""){
			Artvert alias = artvert.getAlias();
			downloadArtvert(alias);
			return;
		}

		ofxHttpResponse resp_detector = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.detector_data");
		if(resp_detector.status==200){
			ofLog(OF_LOG_VERBOSE,"got detector data correctly, saving " + ofToString(resp_detector.responseBody.size()) + " bytes");
			ofFile & detectorData = artvert.getDetectorData();
			detectorData.changeMode(ofFile::WriteOnly,true);
			detectorData << resp_detector.responseBody;
			detectorData.changeMode(ofFile::ReadOnly,true);
		}else if(resp_detector.status==301){
			ofLog(OF_LOG_VERBOSE,"got detector redirection " + resp_detector.location);
			string aliasUID = ofFilePath::getBaseName(ofFilePath::getBaseName(resp_detector.location));
			artvert.setAliasUID(aliasUID);

			Artvert alias = artvert.getAlias();
			downloadArtvert(alias);

			artvert.save();
			PersistanceEngine::save();
			return;
		}
	}

	if(!artvert.getTrackerData().exists()){
		ofxHttpResponse resp_tracker = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.tracker_data");
		if(resp_tracker.status==200){
			ofLog(OF_LOG_VERBOSE,"got tracker data correctly, saving " + ofToString(resp_tracker.responseBody.size()) + " bytes");
			ofFile & trackerData = artvert.getTrackerData();
			trackerData.changeMode(ofFile::WriteOnly,true);
			trackerData << resp_tracker.responseBody;
			trackerData.changeMode(ofFile::ReadOnly,true);
		}
	}

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
				if(!checkUploaded(artverts[i])){
					ofxHttpResponse response = postAdvert(artverts[i]);
					if(response.status!=200){
						ofLog(OF_LOG_ERROR,"error sending " + artverts[i].getUID() + ":  " + response.reasonForStatus);
					}
				}
				if(checkAnalized(artverts[i])){
					downloadAnalisys(artverts[i]);
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
