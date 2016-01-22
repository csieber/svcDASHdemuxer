#include "../../h264Error.hpp"

#ifndef H264SVCERROR_HPP_
#define H264SVCERROR_HPP_

namespace H264 {

struct h264SvcInvalidNal : virtual h264Error {

	virtual const char* what() const throw() {
		return "Invalid SVC NAL unit!";
	};
};

};

#endif /* H264SVCERROR_HPP_ */
