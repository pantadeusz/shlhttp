/**
 *
 * Single header http server
 *
 * */

//#include <cstring>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

#include <netdb.h>
#include <unistd.h>

namespace shttp {

auto normalize_header_key = []( const std::string &data0 ) {
	auto data = data0;
	std::transform( data.begin(), data.end(), data.begin(),	[]( char c )->char{return std::tolower( c );} );
	return data;
};

inline std::vector<std::string> split_by_char( std::string s, char d ) {
	std::stringstream ss( s );
	std::string item;
	std::vector<std::string> items;
	while ( std::getline( ss, item, ';' ) ) {
		items.push_back( item );
	}
	return items;
}


class headers_t : public std::list<std::pair<std::string, std::string>> {
public:
	std::string &operator[]( const std::string &key ) {
		for ( auto &[k, v] : *this ) {
			if ( k == normalize_header_key( key ) )
				return v;
		}
		this->push_back( {normalize_header_key( key ), ""} );
		return back().second;
	}


	int count( const std::string key ) const {
		int ret = 0;
		for ( auto &[k, v] : *this ) {
			if ( k == normalize_header_key( key ) )
				ret++;
		}
		return ret;
	}
};

std::ostream &operator<<( std::ostream &os, headers_t const &value ) {
	for ( auto &[k, v] : value ) {
		os << normalize_header_key( k ) << ": " << v << "\r\n";
	}
	return os;
}

std::istream &operator>>( std::istream &is, headers_t &value ) {
	std::string line;
	while ( getline( is, line, '\n' ) ) {
		if ( ( line == "\r" ) || ( line.size() == 0 ) )
			return is;
		line.pop_back();
		std::regex headers_regex( "^([^\\:]*)[\\:] *(.*)$" );
		std::smatch header_match;
		if ( std::regex_search( line, header_match, headers_regex ) ) {
			value[normalize_header_key( header_match[1] )] = header_match[2];
		}
	}
	return is;
}

class request_t {
public:
	std::string method;
	std::string path;
	std::string version;
	headers_t headers;
	std::vector<char> body;
};

std::ostream &operator<<( std::ostream &os, request_t const &request ) {
	os << request.method << " " << request.path << " " << request.version
	   << "\r\n";
	os << request.headers;
	os << "\r\n";
	if ( request.body.size() > 0 ) {
		os << std::string( request.body.begin(), request.body.end() );
		os << "\r\n";
	}
	return os;
}

std::istream &operator>>( std::istream &is, request_t &request ) {
	std::string line;
	getline( is, line, '\n' );
	if ( ( line == "\r" ) || ( line.size() == 0 ) )
		return is;
	line.pop_back();
	std::regex headers_regex( "^([A-Z]+) ([^ ]*) (HTTP.*)$" );
	std::smatch header_match;
	if ( std::regex_search( line, header_match, headers_regex ) ) {
		request.method = header_match[1];
		request.path = header_match[2];
		request.version = header_match[3];
	}
	is >> request.headers;
	char c;
	while ( is >> c ) {
		request.body.push_back( c );
	}
	return is;
}

using request_p = std::shared_ptr<request_t>;

inline bool operator==( const request_t &l, const request_t &r ) {
	if ( l.method != r.method )
		return false;
	if ( l.path != r.path )
		return false;
	if ( l.version != r.version )
		return false;
	if ( l.headers != r.headers )
		return false;
	if ( l.body != r.body )
		return false;
	return true;
}

struct response_t {
	std::string version;
	int status_code;
	std::string status_message;
	headers_t headers;
	std::vector<char> body;
};
using response_p = std::shared_ptr<response_t>;

inline response_t response_of ( int status_code, std::string status_message, std::string body ) {
	response_t response;
	response.status_code = status_code;
	response.status_message = status_message;
	response.version = "HTTP/1.1";
	response.body = std::vector<char>( body.begin(), body.end() );
	return response;
};

std::ostream &operator<<( std::ostream &os, response_t const &response ) {
	os << response.version << " " << response.status_code << " "
	   << response.status_message << "\r\n";
	os << response.headers;
	os << "\r\n";
	if ( response.body.size() > 0 ) {
		os << std::string( response.body.begin(), response.body.end() );
	}
	return os;
}

inline int listen_socket( std::string host = "localhost", std::string port = "8080" ) {
	int yes = 1;
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s;
	int sdd;

	for ( unsigned int i = 0; i < sizeof( struct addrinfo ); i++ )
		( ( char * )&hints )[i] = 0;
	hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
	hints.ai_protocol = 0;           /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	s = getaddrinfo( host.c_str(), port.c_str(), &hints, &result );
	if ( s != 0 ) {
		std::stringstream ss;
		ss << "ListeningSocket getaddrinfo:: " << gai_strerror( s )
		   << "; port= " << port;
		throw std::invalid_argument( ss.str() );
	}

	for ( rp = result; rp != NULL; rp = rp->ai_next ) {
		sdd = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
		if ( sdd == -1 )
			continue;

		if ( setsockopt( sdd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) ) == -1 ) {
			throw std::invalid_argument(
				"error at: setsockopt( sfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof( int "
				") )" );
		}

		if ( bind( sdd, rp->ai_addr, rp->ai_addrlen ) == 0 ) {
			break;
		}
		::close( sdd );
	}

	if ( rp == NULL ) {
		std::stringstream ss;
		ss << "ListeningSocket unable to bind address:: " << gai_strerror( s ) << "; "
		   << port;
		throw std::invalid_argument( ss.str() );
	}
	freeaddrinfo( result );
	if ( listen( sdd, 32 ) == -1 ) {
		throw std::invalid_argument( "listen error" );
	}
	return sdd;
};

inline std::tuple<int, std::string, std::string> accept_on_socket( int sfd ) {
	struct sockaddr_storage peer_addr;
	socklen_t peer_addr_len = sizeof( struct sockaddr_storage );

	std::string host_s;
	std::string service_s;
	int csfd;

	if ( ( csfd = ::accept( sfd, ( struct sockaddr * )&peer_addr, &peer_addr_len ) ) ==
			-1 ) {
		throw std::invalid_argument( "could not accept connection!" );
	} else {
		char host[NI_MAXHOST], service[NI_MAXSERV];

		// int s =
		getnameinfo( ( struct sockaddr * )&peer_addr, peer_addr_len, host, NI_MAXHOST,
					 service, NI_MAXSERV, NI_NUMERICSERV );
		host_s = host;
		service_s = service;
	}
	return {csfd, host_s, service_s};
};

inline request_t receive_request( int sockfd ) {
	std::string input_buffer;
	char buf[1];
	request_t request;
	while ( recv( sockfd, buf, 1, 0 ) > 0 ) {
		input_buffer.push_back( buf[0] );
		if ( input_buffer.size() > 4 ) {
			if ( input_buffer.substr( input_buffer.size() - 4, 4 ) == "\r\n\r\n" ) {
				std::stringstream ss( input_buffer );
				ss >> request;
				break;
			}
		}
	}
	// now we have request headers. We shall interpret them
	if ( request.method == "GET" )
		return request;

	if ( request.headers.count( "content-length" ) > 0 ) {
		int content_length = std::stoi( request.headers["content-length"] );
		for ( int i = 0; ( i < content_length ) && ( recv( sockfd, buf, 1, 0 ) > 0 ); i++ )
			request.body.push_back( buf[0] );
		return request;
	} else {
		while ( recv( sockfd, buf, 1, 0 ) > 0 ) {
			input_buffer.push_back( buf[0] );
		}
		std::stringstream ss( input_buffer );
		ss >> request;
		return request;
	}
};

auto send_response = []( int sockfd, response_t response ) {
	std::stringstream ss;
	if ( response.body.size() )
		response.headers["content-length"] = std::to_string( response.body.size() );
	ss << response;
	std::string str = ss.str().c_str();
	send( sockfd, str.data(), str.size(), 0 );
};

#define HTTP_SIMPLE_IMPLEMENTATION
#ifdef HTTP_SIMPLE_IMPLEMENTATION

#endif

} // namespace shttp
