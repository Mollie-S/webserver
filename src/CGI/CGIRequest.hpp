#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

// #include "../HTTPRequest/RequestMessage.hpp"
// #include "../HTTPResponse/SpecifiedConfig.hpp"

class CGIRequest
{
private:
	char *_envp[21];				// TODO check if this is the right size, terminated with 0
	char *_argument[3];
	std::map<std::string, std::string> _meta_variables;
	std::string _cgi_name;
	std::vector<std::string> _cgi_extension;
	bool _search_cgi_extension;

public:
	CGIRequest();
	~CGIRequest();
	// void parse_meta_variables(HTTPRequest::RequestMessage request_message);//TODO update later
	// void parse_meta_variables(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig _config);
	void search_cgi(std::vector<std::string> &path);
	void parse_meta_variables(void);
	void set_envp(void);
	void set_argument(std::string cgi_path);
	// char **get_argument();
	// int execute_cgi(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig _config, int fd);
	// int execute_cgi(HTTPRequest::RequestMessage *_http_request_message, HTTPResponse::SpecifiedConfig _config);
	int execute_cgi(void);
};
