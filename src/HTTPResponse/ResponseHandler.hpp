#pragma once

#include <string>

#include "StatusCodes.hpp"
#include "../config/ServerBlock.hpp"
#include "../HTTPRequest/RequestMessage.hpp"
#include "ResponseMessage.hpp"
#include "SpecifiedConfig.hpp"

namespace HTTPResponse
{
	class ResponseHandler
	{
	private:
		HTTPRequest::RequestMessage *_http_request_message;
		ResponseMessage *_http_response_messa
		SpecifiedConfig _config;

		bool _verify_method(const std::vector<std::string> methods);
		const std::string& _create_allowed_methods_line(const std::vector<std::string> methods);
		bool _check_client_body_size();
		void _handle_error(HTTPResponse::StatusCode code);
		void _build_final_response();

	public:
		ResponseHandler(HTTPRequest::RequestMessage *request_message, ResponseMessage *response_message);
		~ResponseHandler();

		void create_http_response();
		const std::string convert_status_code_to_string(const int code);
		void set_config_rules(const Config::ServerBlock *virtual_server, const Config::LocationBlock *location);
		// TODO do i need getter if I think i wont need it  at all outside of the class?
	};
}
