#include "NALStream.h"

namespace H264 {

NALStream::NALStream() {
	m_iPosNAL = 0;
}

NALStream::~NALStream() {
}

/**
 * Creates an NAL index for the stream.
 *
 * @throws h264NALInvalidNALStream
 */
bool NALStream::createIndex()
{
	return false;
}

bool NALStream::indexLoaded()
{
	return false;
}

bool NALStream::loadIndex()
{
	return false;
}

bool NALStream::saveIndex()
{
	return false;
}

bool NALStream::seek(int32_t iNAL)
{
	return false;
}

bool NALStream::rewind()
{
	return false;
}

raw_data NALStream::getNAL(int32_t iNal)
{
	return raw_data(boost::shared_array<char>(), -1);
}

int32_t NALStream::nalIndexSize()
{
	return -1;
}

}
