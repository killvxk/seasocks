#include "internal/PageRequest.h"

#include <cstring>

namespace {

using namespace SeaSocks;

Request::Verb lookup(const char* verb) {
	if (std::strcmp(verb, "GET") == 0) return Request::Get;
	if (std::strcmp(verb, "PUT") == 0) return Request::Put;
	if (std::strcmp(verb, "POST") == 0) return Request::Post;
	if (std::strcmp(verb, "DELETE") == 0) return Request::Delete;
	return Request::Invalid;
}

}

namespace SeaSocks {

PageRequest::PageRequest(
		boost::shared_ptr<SeaSocks::Credentials> credentials,
		const sockaddr_in& remoteAddress,
		const std::string& requestUri,
		const char* verb,
		size_t contentLength,
		const std::map<std::string, std::string>& headers) :
			_credentials(credentials),
			_remoteAddress(remoteAddress),
			_requestUri(requestUri),
			_verb(lookup(verb)),
			_contentLength(contentLength),
			_headers(headers) {
}

bool PageRequest::consumeContent(std::vector<uint8_t>& buffer) {
	if (buffer.size() < _contentLength) return false;
	if (buffer.size() == _contentLength) {
		_content.swap(buffer);
	} else {
		_content.assign(buffer.begin(), buffer.begin() + _contentLength);
		buffer.erase(buffer.begin(), buffer.begin() + _contentLength);
	}
	return true;
}

}  // namespace SeaSocks
