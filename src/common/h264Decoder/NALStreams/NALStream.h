#ifndef NALSTREAM_H_
#define NALSTREAM_H_

#include <stdint.h>
#include <boost/tuple/tuple.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <list>
#include "../types.hpp"

namespace H264 {

class NALStream {

public:

	typedef boost::shared_ptr<NALStream> ptr;

	enum feature_t {

		/*
		 * Specifies whether the stream is NAL forward seekable.
		 * That does not mean the stream is byte seekable!
		 */
		SEEKABLE_FORWARD,

		/*
		 * Specifies whether the stream is NAL backward seekable.
		 * That does not mean the stream is byte seekable!
		 */
		SEEKABLE_BACKWARD,

		/*
		 * Specifies whether the stream can create a NAL index.
		 */
		INDEXABLE,
	};

	NALStream();
	virtual ~NALStream();

	/*
	 * Basic functions
	 */

	virtual bool feature(feature_t feature) = 0;

	virtual raw_data next() = 0;

	virtual bool close() = 0;

	int32_t posNal() { return m_iPosNAL; };

	/*
	 * Functions requiring seeking and indexing
	 */
	virtual bool seek(int32_t iNAL);
	virtual bool rewind();

	virtual bool createIndex();
	virtual bool indexLoaded();
	virtual bool loadIndex();
	virtual bool saveIndex();

	virtual raw_data getNAL(int32_t iNal);

	virtual int32_t nalIndexSize();

protected:
	int32_t m_iPosNAL;
};

}

#endif /* NALSTREAM_H_ */
