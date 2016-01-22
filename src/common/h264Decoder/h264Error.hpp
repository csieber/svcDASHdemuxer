#ifndef H264ERROR_HPP
#define H264ERROR_HPP

#include <boost/exception/all.hpp>

namespace H264 {

struct h264Error: virtual std::exception, virtual boost::exception {
  virtual const char *what() const throw() { return "Unspecified h264 decoder error."; }
};

/*
 * Exceptions
 */

typedef boost::error_info<struct errinfo_message_, std::string> errinfo_message;

struct h264FileNotFoundError : virtual h264Error {

	virtual const char* what() const throw()
			{ return "File not found!"; };
};

struct h264FileStreamAccessError : virtual h264Error {

	virtual const char* what() const throw()
			{ return "File access failed!"; };
};

struct h264IndexError : virtual h264Error {

	virtual const char* what() const throw()
				{ return "Index error!"; };
};

struct h264NALInvalidNALStream : virtual h264Error {

	virtual const char* what() const throw()
			{ return "NAL stream encountered an error."; };
};

struct h264InvalidNal : virtual h264Error {

	virtual const char* what() const throw() {
		return "Invalid NAL unit!";
	};
};

struct h264InvalidRbsp : virtual h264Error {

	virtual const char* what() const throw() {
		return "Invalid RBSP data!";
	};
};

struct h264DbReadonly : virtual h264Error {

	virtual const char* what() const throw() {
		return "Nal db is read-only!";
	};
};

struct h264RequestedNALNotAvailable : virtual h264Error {

	virtual const char* what() const throw() {
		return "Requested NAL unit is not available!";
	};
};

struct h264RequestedFrameNotAvailable : virtual h264Error {

	virtual const char* what() const throw() {
		return "Requested NAL unit is not available!";
	};
};

};

#endif //H264ERROR_HPP
