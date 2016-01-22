#include <boost/utility.hpp>
#include <boost/tuple/tuple.hpp>
#include "../types.hpp"
#include "types_svc.hpp"

#ifndef STATICNALPARSERSVC_H_
#define STATICNALPARSERSVC_H_

namespace H264Parser {

class StaticNALParserSVC
	: boost::noncopyable {

public:
	StaticNALParserSVC();
	virtual ~StaticNALParserSVC();

	static nal_hdr_svc parseSvcHdr(raw_data nal_ptr, nal_hdr hdr);

	static bool isSvcNal(nal_hdr hdr);

	static slice_header_svc parseSliceHeader(raw_data rbsp_ptr);
};

}

#endif /* STATICNALPARSERSVC_H_ */
