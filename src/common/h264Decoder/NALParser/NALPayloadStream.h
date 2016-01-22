#ifndef NALPAYLOADSTREAM_H
#define NALPAYLOADSTREAM_H

#include <boost/dynamic_bitset.hpp>
#include "types.hpp"
#include "../types.hpp"

namespace H264Parser {

/**
 * Reads/decodes values from a payload-rbsp-bitstream.
 * RBSP = raw byte sequence payload (RBSP).
 *
 * \author sieber@informatik.uni-wuerzburg.de
 */
class NALPayLoadStream {

public:
	NALPayLoadStream(raw_data rbsp);
	~NALPayLoadStream();

    bool readFlag();
    unsigned int readUE(); /* ue(v): unsigned integer Exp-Golomb-coded syntax */
    int          readSE(); /* se(v): signed integer Exp-Golomb-coded syntax */
    unsigned int readBits(unsigned int n);
    unsigned int nextBits(unsigned int n);
    bool         moreRbspData();
    bool         byte_aligned();

private:
    unsigned int BitNumValue(bool bits[], unsigned int size);
    bool         moreRbspTrailingData();
    bool		 bit(unsigned int iPos);

    boost::dynamic_bitset<unsigned char>* m_pPayload;
    unsigned int m_iPos;
};

}
#endif // NALPAYLOADSTREAM_H
