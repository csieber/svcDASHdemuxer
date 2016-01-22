#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "types.hpp"
#include "NALStreams/NALStream.h"

#ifndef H264DECODER_H_
#define H264DECODER_H_

namespace H264 {

class h264Decoder {
public:

	typedef boost::shared_ptr<h264Decoder> ptr;

	enum proto_t {
		TCP,
		UDP
	};

	enum pos_t {
		FILE,
		NAL,
		FRAME
	};

	h264Decoder(NALStream* pStream);
	virtual ~h264Decoder();

	nal_short_info next();

	bool close();
	bool eof();

	NALStream* stream() { return m_pStream; };

private:
	nal_short_info parseNAL(raw_data nal);

	NALStream* m_pStream;

	int32_t frame_num_counter;

	// Used to be able to count the frames correctly
	nal_short_info last_prefix_nal_;
};

}

#endif /* H264DECODER_H_ */
