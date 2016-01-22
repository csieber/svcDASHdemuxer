#ifndef H264STREAMSTATS_H_
#define H264STREAMSTATS_H_

#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <list>
#include "types.hpp"

namespace H264 {

struct h264_layer {

	h264_layer() {

		temporal_id = -1;
		dependency_id = -1;
		quality_id = -1;

		numb_of_bytes = 0;
		numb_of_nals = 0;
	};

	int32_t temporal_id;
	int32_t dependency_id;
	int32_t quality_id;

	int32_t numb_of_bytes;
	int32_t numb_of_nals;

	h264_layer& operator=(nal_short_info& nal) {
		temporal_id = nal.temporal_id;
		dependency_id = nal.dependency_id;
		quality_id = nal.quality_id;

		return *this;
	}

	bool operator==(h264_layer& lay) {

		if (this->dependency_id == lay.dependency_id &&
			this->temporal_id == lay.temporal_id &&
			this->quality_id == lay.quality_id)
			return true;
		else
			return false;
	}

	bool operator==(nal_short_info& nal) {

		if (this->dependency_id == nal.dependency_id &&
			this->temporal_id == nal.temporal_id &&
			this->quality_id == nal.quality_id)
					return true;
				else
					return false;
	}
};

class h264StreamStats
	: boost::noncopyable {
public:

	typedef boost::shared_ptr<h264StreamStats> ptr;

	h264StreamStats();
	virtual ~h264StreamStats();

	h264StreamStats& operator<<(nal_short_info info);

	bool save(std::string sFile);
	bool load(std::string sFile);

	void reset();

//private:
	int64_t m_iBytesTotal;
	int32_t m_iNumbOfNals;
	int32_t m_iNumbOfParsingErrors;
	int32_t m_iNumbOfFrames;

	int32_t m_iLastFrameNr;
	int32_t m_iHighestFrameNr;

	std::list<h264_layer> m_layers;
	int32_t m_last_layer[3];
};

}

#endif /* H264STREAMSTATS_H_ */
