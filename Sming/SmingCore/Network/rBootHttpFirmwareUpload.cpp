/* This file is part of Sming Framework Project
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * This file is provided under the LGPL v3 license.
 *
 * File Author: Patrick Jahns (https://github.com/patrickjahns)
 *
 * Created on April 12, 2016
 */
#include "rBootHttpFirmwareUpload.h"


rBootHttpFirmwareUpload::rBootHttpFirmwareUpload() {
	_server = NULL;
	_username = "";
	_password = "";
	_secured = false;
};
rBootHttpFirmwareUpload::~rBootHttpFirmwareUpload() {
	_server = NULL;
};

void rBootHttpFirmwareUpload::enable(HttpServer* server, String path, bool spiffs /* = false */) {
	_server = server;
	_path = path
	_server->addPath(_path, HttpPathDelegate(&rBootHttpFirmwareUpload::onUploadRequest, this),
							HttpPathDelegate(&rBootHttpFirmwareUpload::uploadHandler, this));
}

void rBootHttpFirmwareUpload::enable(HttpServer* server, String path, String password, bool spiffs /* = false */) {
	_password = password;
	_secured = true;
	rBootHttpFirmwareUpload(server, path, spiffs);
}

void rBootHttpFirmwareUpload::enable(HttpServer* server, String path, String username, String password, bool spiffs /* = false */) {
	_username = username;
	_password = password;
	_secured = true;
	rBootHttpFirmwareUpload(server, path, spiffs);
}




void rBootHttpFirmwareUpload::disable() {
	; //ToDo need removePath function
	_server = NULL;
	_username = "";
	_password = "";
	_secured = false;
}


void rBootHttpFirmwareUpload::onUploadRequest(HttpRequest &request, HttpResponse &response) {
	/*
	 if(_secured && !request.isAuthenticated(username, password))
	{
		response.authorizationRequired();
		return;
	}

	*/

	String body = "<html>\r\n";
	body += "<head>\r\n";
	body += "<title>SMING HTTP Firmware Upload</title>\r\n";
	body += "</head>\r\n";
	body += "<body><br>\r\n";
	if(request.getRequestMethod() == RequestMethod::GET)
	{

		body += "<form method='POST' action='"+ _path +"' enctype='multipart/form-data'>\r\n";
		body += "<p>Rom:&nbsp;&nbsp;&nbsp;<input type='file' name='rom'></p>\r\n";
		if(_spiffs)
		{
			body += "<p>Spiffs:&nbsp;<input type='file' name='spiffs'></p>\r\n";
		}
		body += "<p><input type='submit' value='Upload'></p>\r\n";
		body += "</form>\r\n";

	}
	else if (request.getRequestMethod() == RequestMethod::POST)
	{
		body += "success";
		// TODO: provide restart button here?
		// TODO: automatically restart?
	}
	else
	{
		response.badRequest();
		return;
	}

	body += "</body>\r\n";
	body += "</html>\r\n";

	response.setContentType(ContentType::HTML);
	response.sendString(body);
}


void rBootHttpFirmwareUpload::uploadHandler(HttpRequest& request, HttpUpload& upload) {
	//TODO: enable authorization
	//TODO: can we intercept the file transfer beforehand and send a custom response here?
	/*
	 if(_secured && !request.isAuthenticated(username, password))
	{
		upload.status == HTTP_UPLOAD_SKIP;
	}

	*/
	if(upload.status == HTTP_UPLOAD_BEGIN)
	{
		debugf("begin upload");
		// only allow spiffs or rom - ignore other variable names
		if(upload.name.equals("spiffs") || upload.name.equals("rom"))
		{
			upload.status == HTTP_UPLOAD_WRITE_CUSTOM;
		}
		else
		{
			upload.status == HTTP_UPLOAD_SKIP;
		}
	}
	else if(upload.status == HTTP_UPLOAD_WRITE_CUSTOM)
	{


	}
	else if(upload.status == HTTP_UPLOAD_FINISHED)
	{
		debugf("upload finished");
	}
	else if(upload.status == HTTP_UPLOAD_ABORT)
	{



	}
}
