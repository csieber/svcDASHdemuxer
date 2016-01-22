#include <boost/utility.hpp>
#include <boost/tuple/tuple.hpp>
#include "types.hpp"

#ifndef STATICNALPARSER_H_
#define STATICNALPARSER_H_

namespace H264Parser {

class StaticNALParser
	: boost::noncopyable  {

public:

	static nal_hdr parseNalHdr(raw_data nal_ptr);

	static raw_data extractRbspData(raw_data nal_ptr, int32_t hdr_len);

	static slice_header parseSliceHeader(raw_data rbsp_ptr);

private:
	StaticNALParser();
	virtual ~StaticNALParser();
};

}

#endif /* STATICNALPARSER_H_ */
