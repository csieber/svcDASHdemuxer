#ifndef H264STREAMDB_H_
#define H264STREAMDB_H_

#include <boost/utility.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <vector>
#include <list>
#include <map>
#include "types.hpp"
#include <string>

namespace H264 {

class h264StreamDb
	: boost::noncopyable {
public:

	typedef boost::shared_ptr<h264StreamDb> ptr;

	h264StreamDb();
	virtual ~h264StreamDb();

	bool load(std::string sFile);
	bool save(std::string sFile);

	void reset();

	h264StreamDb& operator<<(nal_short_info nal);

	const nal_short_info& operator[](int32_t nal_nr);

	std::list<nal_short_info> frame(const int32_t& frame_nr);

	int32_t frame_count() { return m_firstNalOfFrame.size(); };

	int32_t packetCountForPayloadSize(int32_t max_payload_size);

private:
	void push_back(nal_short_info& info);

	FILE* m_pFile;
	std::string m_sFile;

	std::vector<nal_short_info> m_nalDb;

	// <frame_nr, nal_nr>
	std::map<int32_t, int32_t> m_firstNalOfFrame;
	int32_t m_lastFrameNr;
};

}

#endif /* H264STREAMDB_H_ */
