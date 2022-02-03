#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "shttp.hpp"

#include <sstream>

using namespace shttp;

TEST_CASE( "request_t", "[request_t]" ) {
	auto request = std::make_shared<request_t>();
	request->method = "GET";
	request->path = "/";
	request->version = "HTTP/1.1";
	request->headers["Host"] = "localhost:5000";
	request->body = {};
	SECTION( "request_to_data generates correct data vector" ) {
		std::stringstream data_to_send;
		data_to_send << *request;
		std::string string_to_send = data_to_send.str();
		REQUIRE( string_to_send == "GET / HTTP/1.1\r\nhost: localhost:5000\r\n\r\n" );
	}

	SECTION( "two requests can be compared" ) {
		auto request2 = std::make_shared<request_t>();
		request2->method = "GET";
		request2->path = "/";
		request2->version = "HTTP/1.1";
		request2->headers["Host"] = "localhost:5000";
		request2->body = {};
		REQUIRE( *request == *request2 );

		*request2 = *request;
		request2->method = "POST";
		REQUIRE_FALSE( *request == *request2 );
		*request2 = *request;
		request2->path = "/xx";
		REQUIRE_FALSE( *request == *request2 );
		*request2 = *request;
		request2->version = "HTTP/1.0";
		REQUIRE_FALSE( *request == *request2 );
		*request2 = *request;
		request2->headers.clear();
		REQUIRE_FALSE( *request == *request2 );
		*request2 = *request;
		request2->headers["xx"] = "dsdf";
		REQUIRE_FALSE( *request == *request2 );
		*request2 = *request;
		request2->body = {'d', 'w'};
		REQUIRE_FALSE( *request == *request2 );
	}

	SECTION( "generate request from string" ) {
		std::string req_str = "GET / HTTP/1.1\r\nhost: localhost:5000\r\n\r\n";
		std::stringstream ss( req_str );
		request_t req;
		ss >> req;
		std::cout << req;
	}
}
