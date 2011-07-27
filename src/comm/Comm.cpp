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
		ofLogVerbose("Comm", artvert.getUID() + " already uploaded ");
	}else{
		ofLogVerbose("Comm", artvert.getUID() + " not uploaded yet ");
	}

	return response.status == 200;
}

bool Comm::checkAnalized(Artvert & artvert){
	ofxHttpForm form;
	form.action = url+"/checkanalized.of";
	form.addFormField("uid",artvert.getUID());
	ofLogVerbose("Comm", "checking analized " + artvert.getUID());
	ofxHttpResponse response = httpClient.submitForm(form);
	return response.status==200;
}

void Comm::downloadArtvert(Artvert & artvert){
	if(!artvert.getCompressedImage().exists()){
		ofxHttpResponse resp_compressed = httpClient.getUrl(url+"/"+artvert.getUID()+".jpg");
		if(resp_compressed.status==200){
			ofLogVerbose("Comm", "got jpg correctly, saving " + ofToString(resp_compressed.responseBody.size()) + " bytes");
			ofFile compressed = artvert.getCompressedImage();
			compressed.changeMode(ofFile::WriteOnly,true);
			compressed << resp_compressed.responseBody;
			compressed.close();

			ofImage model;
			model.setUseTexture(false);
			model.loadImage(compressed.getAbsolutePath());
			model.saveImage(artvert.getModel());
		}
	}

	if(!artvert.getROIFile().exists()){
		ofxHttpResponse resp_roi = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.roi");
		if(resp_roi.status==200){
			ofLogVerbose("Comm", "got roi correctly, saving " + ofToString(resp_roi.responseBody.size()) + " bytes");
			ofFile roi = artvert.getROIFile();
			roi.changeMode(ofFile::WriteOnly,true);
			roi << resp_roi.responseBody;
			roi.close();
		}
	}

	if(!artvert.getMD5File().exists()){
		ofxHttpResponse resp_md5 = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.md5.notready");
		if(resp_md5.status==200){
			ofLogVerbose("Comm", "got md5 correctly, saving " + ofToString(resp_md5.responseBody.size()) + " bytes");
			ofFile md5(artvert.getMD5File().path()+".notready");
			md5.changeMode(ofFile::WriteOnly,true);
			md5 << resp_md5.responseBody;
			md5.close();
			md5.moveTo(artvert.getMD5File().path());
			if(!artvert.checkIntegrity()){
				ofLogWarning("Comm", "error md5 doesn't match for " + artvert.getUID() + " will retry later");
				ofLogVerbose("Comm", "downloaded md5: " + artvert.getStoredMD5());
				ofLogVerbose("Comm", "generated md5: " + artvert.generateMD5());
				artvert.remove();
			}
		}else{
			ofLogWarning("Comm", "couldn't download md5 for " + artvert.getUID() + " will retry later");
		}
	}

	downloadAnalisys(artvert);

}

void Comm::downloadAnalisys(Artvert & artvert){
	ofLogVerbose("Comm", "downloading analysis for " + artvert.getUID());
	if(!artvert.getDetectorData().exists()){
		if(artvert.getAliasUID()!=""){
			ofLogVerbose("Comm", "redirecting to " + artvert.getAliasUID());
			Artvert alias = artvert.getAlias();
			downloadArtvert(alias);
			return;
		}

		ofxHttpResponse resp_detector = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.detector_data");
		if(resp_detector.status==200){
			ofLogVerbose("Comm", "got detector data correctly, saving " + ofToString(resp_detector.responseBody.size()) + " bytes");
			ofFile detectorData = artvert.getDetectorData();
			detectorData.changeMode(ofFile::WriteOnly,true);
			detectorData << resp_detector.responseBody;
			detectorData.close();
		}else if(resp_detector.status==301){
			ofLogVerbose("Comm", "got detector redirection " + resp_detector.location);
			string aliasUID = ofFilePath::getBaseName(ofFilePath::getBaseName(resp_detector.location));
			artvert.setAliasUID(aliasUID);

			Artvert alias = artvert.getAlias();
			downloadArtvert(alias);

			artvert.save();
			PersistanceEngine::save();
			return;
		}else{
			ofLogWarning("Comm", "couldn't download detection data for " + artvert.getUID() + " will retry later");
		}
	}

	if(!artvert.getTrackerData().exists()){
		ofxHttpResponse resp_tracker = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.tracker_data");
		if(resp_tracker.status==200){
			ofLogVerbose("Comm", "got tracker data correctly, saving " + ofToString(resp_tracker.responseBody.size()) + " bytes");
			ofFile trackerData = artvert.getTrackerData();
			trackerData.changeMode(ofFile::WriteOnly,true);
			trackerData << resp_tracker.responseBody;
			trackerData.close();
		}else{
			ofLogWarning("Comm", "couldn't download tracker data for " + artvert.getUID() + " will retry later");
		}
	}

	ofxHttpResponse resp_md5 = httpClient.getUrl(url+"/"+artvert.getUID()+".bmp.md5");
	if(resp_md5.status==200){
		ofLogVerbose("Comm", "got md5 correctly, saving " + ofToString(resp_md5.responseBody.size()) + " bytes");
		ofFile md5 = artvert.getMD5File();
		md5.changeMode(ofFile::WriteOnly,true);
		md5 << resp_md5.responseBody;
		md5.close();
		if(!artvert.checkIntegrity()){
			ofLogWarning("Comm", "error md5 doesn't match for " + artvert.getUID() + " will retry later");
			ofLogVerbose("Comm", "downloaded md5: " + artvert.getStoredMD5());
			ofLogVerbose("Comm", "generated md5: " + artvert.generateMD5());
			artvert.removeAnalisys();
		}
	}else{
		ofLogWarning("Comm", "couldn't download md5 for " + artvert.getUID() + " will retry later");
	}

}

void Comm::threadedFunction(){
	vector<Artvert> artverts = Artvert::listAll();
	for(int i=0;i<(int)artverts.size();i++){
		if(!artverts[i].isReady() && !checkUploaded(artverts[i])){
			ofxHttpResponse response = postAdvert(artverts[i]);
			if(response.status!=200){
				ofLogError("Comm", "error sending " + artverts[i].getUID() + ":  " + response.reasonForStatus);
			}
		}
	}

	while(isThreadRunning()){
		vector<Artvert> artverts = Artvert::listAll();
		for(int i=0;i<(int)artverts.size();i++){
			if(!artverts[i].isReady()){
				if(checkAnalized(artverts[i])){
					downloadAnalisys(artverts[i]);
					if(artverts[i].isReady()){
						ofNotifyEvent(gotAnalysisE,artverts[i],this);
					}
				}else if(!checkUploaded(artverts[i])){
					ofxHttpResponse response = postAdvert(artverts[i]);
					if(response.status!=200){
						ofLogError("Comm", "error sending " + artverts[i].getUID() + ":  " + response.reasonForStatus);
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
	form.addFile("location",artvert.getLocationFile().getAbsolutePath());
	form.addFile("md5",artvert.getMD5File().getAbsolutePath());
	form.method = OFX_HTTP_POST;

	return httpClient.submitForm(form);
}

void Comm::sendAdvert(const Artvert & artvert){
	ofxHttpForm form;
	form.action = url+"/postadvert.of";
	form.addFormField("uid",artvert.getUID());
	form.addFile("artvert",artvert.getCompressedImage().getAbsolutePath());
	form.addFile("roi",artvert.getROIFile().getAbsolutePath());
	form.addFile("location",artvert.getLocationFile().getAbsolutePath());
	form.addFile("md5",artvert.getMD5File().getAbsolutePath());
	form.method = OFX_HTTP_POST;

	httpClient.addForm(form);
	httpClient.start();
}

void Comm::newResponse(ofxHttpResponse & response){
	ofLogVerbose("Comm") << "new response " << response.url << endl;
	if(response.status!=200){
		ofLogError("Comm") <<  "error response: " << response.status << ":" << response.reasonForStatus << endl;
		ofLogVerbose("Comm") <<  ofVec3f(3,2,1);// << response.responseBody << endl;
		return;
	}else{
		ofLogVerbose("Comm") <<  "correct response: " << response.status <<  + " " + response.url << endl;
		//cout << response.responseBody << endl;
		return;
	}
}
