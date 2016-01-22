#ifndef NALFILESTREAM_H_
#define NALFILESTREAM_H_

#include "NALStream.h"

#include <string>
#include <map>
#include <boost/tuple/tuple.hpp>

namespace H264 {

class NALFileStream: public H264::NALStream {

public:
	NALFileStream();
	virtual ~NALFileStream();

	bool open(std::string sFile);
	bool close();

	bool feature(feature_t feature);

	raw_data next();

	bool seek(int32_t iNAL);

	bool createIndex();
	bool indexLoaded();
	bool loadIndex();
	bool saveIndex();

	bool rewind();

	raw_data getNAL(int32_t iNal);

	int32_t nalIndexSize();

protected:

private:
	raw_data read_NAL(int32_t iNAL);

	FILE* 		m_pFile;
	std::string m_sFile;
	int64_t		m_iFileSize;

	std::map<int32_t, int64_t> m_NALIndex;
};

}

#endif /* NALFILESTREAM_H_ */
