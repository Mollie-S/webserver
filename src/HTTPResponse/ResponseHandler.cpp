#include "ResponseHandler.hpp"
#include "../Utility/Utility.hpp"
#include "../globals.hpp"

#include <sstream> // for converting int to string

namespace HTTPResponse {
	ResponseHandler::ResponseHandler(HTTPRequest::RequestMessage* request_message, ResponseMessage* response_message)
	: _http_request_message(request_message)
	, _http_response_message(response_message)
	{
	}

	ResponseHandler::ResponseHandler(const ResponseHandler& other) {
 		*this = other;
	}

	const ResponseHandler &ResponseHandler::operator=(const ResponseHandler  &other) {
		_http_request_message = other._http_request_message;
		_http_response_message = other._http_response_message;
		_config = other._config;
		_file = other._file;
        return *this;
    }

	ResponseHandler::~ResponseHandler(){}

	void ResponseHandler::create_http_response() {
		// checks before moving on with methods
		if(!_verify_method(_config.get_limit_except()))
			return(handle_error(MethodNotAllowed));
		if (!_check_client_body_size())
			return(handle_error(ContentTooLarge));

		//HTTP method handling
		_handle_methods(); //inside get_and_head, is_cgi, post, delete

		// if(status_code > 300 ?) //some errors are catched after handling methods?
			//_handle_error(status_code);
		// if(not_redirected)
			//build_final_response
	}

	void ResponseHandler::_handle_methods(void) {
		_file.set_path(_config.get_root(), _http_request_message->get_uri().get_path());

		// if (_http_request_message->get_method() == "DELETE")
		// 	//delete_file();
		// else if (_http_request_message->get_method() == "POST")
		// 	//upload_file();
		// else //GET || HEAD
		_serve_file();
	}

	void ResponseHandler::_serve_file(void) {
		//TODO CGI check? where?
		if (!_file.exists()) //get file existence info 
			return handle_error(NotFound);

		if (_file.is_directory()) {
			//if (search_for_index_page())
				//serve the file?
			//else //no index page -> means directory listing
				if (_config.get_autoindex() == ON) {
					//of -> 403 (forbidden) or 404 (not found) error
					//on -> serve directory
						//content-type = get mime type (".html") = we will create this file?
						//create the file content with what's in the directory (into response body)
						// status code = 200
						//build_final_response
				}
		}

		// if (!_file.is_directory()) { // means its a file
		// 	// find the file in dir
		// 		// get file directory path (last of / ?)
		// 		// open directory
		// 		// readdir into dirent
		// 		// match files and push to matched vectors
		// 		// close dir
		// 	//serve the file?
		// 		//check if (!file opens)
		// 			//return(_handle_error(Forbidden))
		// 		//content-type = get mime type (file name)
		// 		// put file content into response body
		// 		// status code = 200
		// 		// build_final_response
		// }
	}

	// void ResponseHandler::_upload_file(void) {
	// 	//error check
	// 	//regular file check

	// 	//create the file
	// 		//open with O_WRONLY | O_CREAT | O_TRUNC
	// 			//check for failure
	// 			//set up response for uploading
	// 			//status code 200
	// }

	// void ResponseHandler::_delete_file(void) {
	// 	//get file data check for errors with stat

	// 	//check if regular file (allowing normal files to be removed)
	// 		//403 error

	// 	//remove the file
	// 		//unlinke
	// 		//check result of unlink for errors (500)

	// 	//status_code = 200
	// 	//create html response to indicate you removed the  file
	// 	//build_final_response
	// }

	void ResponseHandler::handle_error(HTTPResponse::StatusCode code)
	{
		_http_response_message->set_status_code(Utility::to_string(static_cast<int>(code)));
		_http_response_message->set_reason_phrase(HTTPResponse::get_reason_phrase(code));

		if (code == MethodNotAllowed)
			_http_response_message->set_header_element("Allow", _config.get_methods_line());

		//handle custom error pages //TODO needs works with redirection which will be done afterwards
		
		// generate error page
		_http_response_message->set_header_element("Content-Type", "text/html");
		_http_response_message->set_message_body(std::string("<html>\r\n<center><h1>")
								+ _http_response_message->get_status_code() + "</h1><center>"
								+ "</center><h2>" + _http_response_message->get_reason_phrase() + "</h2></center>"
								+ "<hr><center> HungerWeb 1.0 </center>\r\n"
								+ "</html>\r\n");

		_build_final_response();
	}

	void ResponseHandler::_build_final_response()
	{
		std::string response;
		std::string msg_body = _http_response_message->get_message_body();

		// set any remaining headers
		_http_response_message->set_header_element("Server", "HungerWeb/1.0");
		_http_response_message->set_header_element("Date", Utility::get_formatted_date());
		_http_response_message->set_header_element("Content-Length", Utility::to_string(msg_body.length()));
		//get the last-modified info from the File utility and add it to headers

		// build status line
		response += _http_response_message->get_HTTP_version() + " ";
		response += _http_response_message->get_status_code() + " ";
		response += _http_response_message->get_reason_phrase() + "\r\n";

		// add all the headers to response. Format is {Header}: {Header value} \r\n
		for (std::map<std::string, std::string>::const_iterator it = _http_response_message->get_response_headers().begin(); it != _http_response_message->get_response_headers().end(); it++) {
			if (!it->first.empty())
				response += it->first + ": " + it->second;
			response += "\r\n";
		}

		// if body is not empty add it to  response. Format: \r\n {body}
		response += "\r\n";
		if(!msg_body.empty()) //TODO don't forget to clear body if method is HEAD
			response += msg_body;

		//final step
		_http_response_message->append_complete_response(response);
	}

	bool ResponseHandler::_verify_method(const std::vector<std::string> methods) {
		if (methods.empty())
			return true;
		const std::string request_method = _http_request_message->get_method();
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++)
			if (request_method.compare(*it) == 0)
				return true;
		return false;
	}

	bool ResponseHandler::_check_client_body_size() {
		int body_size = _http_request_message->get_message_body().size();
		if (body_size > _config.get_client_max_body_size())
			return false;
		return true;
	}

	void ResponseHandler::set_config_rules(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location) {
		//directive values between levels are generally inherited or replaced, but not added
		//i.e. only if there are no error_page directives defined on the current level, outer level's are inherited
		if (location && location->get_error_page().size())
			_config.set_error_page_value(location->get_error_page()); 
		else
			_config.set_error_page_value(virtual_server->get_error_page());
		if (location && !location->get_is_size_default()) //we decided to overwrite irrespective of the relationship between levels
			_config.set_client_max_body_size(location->get_client_max_body_size());
		else
			_config.set_client_max_body_size(virtual_server->get_client_max_body_size());
		if (!location)
			_config.set_root_value(virtual_server->get_root());
		
		_config.set_return_value(virtual_server->get_return()); //returns are appended within levels
		if(location) { //location specific config rules, appends and overwrites
			_config.set_specific_location(true);
			_config.set_limit_except(location->get_limit_except());
			_config.set_methods_line(location->get_limit_except());
			_config.set_autoindex(location->get_autoindex());
			_config.set_route(location->get_route());
			_config.set_root_value(location->get_root());
			_config.set_return_value(location->get_return()); //FIXME move this text somewhere in create response.  
		}		
	}
}