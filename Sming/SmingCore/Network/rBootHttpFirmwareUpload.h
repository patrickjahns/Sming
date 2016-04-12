/* This file is part of Sming Framework Project
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * This file is provided under the LGPL v3 license.
 *
 * File Author: Patrick Jahns (https://github.com/patrickjahns)
 *
 * Created on April 12, 2016
 */

#ifndef SMINGCORE_NETWORK_RBOOTHTTPUPLOAD_H_
#define SMINGCORE_NETWORK_RBOOTHTTPUPLOAD_H_

#include <HttpRequest.h>
#include <HttpServer.h>
#include <httpResponse.h>
#include <rboot-api.h>

class rBootHttpFirmwareUpload {
public:
	rBootHttpFirmwareUpload();
	virtual ~rBootHttpFirmwareUpload() {};

	void enable(HttpServer* server, String path, bool spiffs = false);
	void enable(HttpServer* server, String path, String password, bool spiffs = false);
	void enable(HttpServer* server, String path, String username, String password, bool spiffs = false);
	void disable(); //ToDo need removePath function

private:
	void onUploadRequest(HttpRequest &request, HttpResponse &response);
	void uploadHandler(HttpRequest& request, HttpUpload& upload);

private:
	HttpServer* _server;
	bool _spiffs;
	bool _secured;
	String _path;
	String _username;
	String _password;
};

#endif //SMINGCORE_NETWORK_RBOOTHTTPUPLOAD_H_
