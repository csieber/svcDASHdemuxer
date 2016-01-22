#include <math.h>
#include "../h264Error.hpp"
#include "NALPayloadStream.h"

namespace H264Parser {

/**
 * Initializes the bitstream. The payload is copied out
 * of the NALUnit object.
 *
 * \param nal Pointer to a NAL unit.
 */
NALPayLoadStream::NALPayLoadStream(raw_data rbsp)
{
    m_pPayload = new boost::dynamic_bitset<unsigned char>();

    const char* crbsp = rbsp.get<0>().get();

    for (int i = 0; i < rbsp.get<1>(); i++) {

        m_pPayload->push_back(crbsp[i] & 0x80);
        m_pPayload->push_back(crbsp[i] & 0x40);
        m_pPayload->push_back(crbsp[i] & 0x20);
        m_pPayload->push_back(crbsp[i] & 0x10);
        m_pPayload->push_back(crbsp[i] & 0x08);
        m_pPayload->push_back(crbsp[i] & 0x04);
        m_pPayload->push_back(crbsp[i] & 0x02);
        m_pPayload->push_back(crbsp[i] & 0x01);
    }

    m_iPos = 0;
}

/**
 * Deconstructor. All associated data is deleted.
 */
NALPayLoadStream::~NALPayLoadStream()
{
    delete m_pPayload;
}

/**
 * Reads a flag (1 bit) from the bitstream.
 *
 * \return The current bit in the bitstream.
 */
bool NALPayLoadStream::readFlag()
{
	if (m_iPos >= m_pPayload->size())
		BOOST_THROW_EXCEPTION(H264::h264InvalidRbsp());

    return bit(m_iPos++);
}

/**
 * Reads a unsigned ex-golomb integer from the bitstream.
 *
 * \return Unsigned integer from the bitstream.
 */
unsigned int NALPayLoadStream::readUE()
{
    unsigned int n = 0;

    while(!bit(m_iPos + n))
        n++;

    m_iPos += n + 1;

    return (unsigned int) (unsigned int)pow(2.0, (double) n) - 1 + readBits(n);
}

/**
 * Reads a signed ex-golomb integer from the bitstream.
 *
 * \return Signed integer.
 */
int NALPayLoadStream::readSE()
{
    unsigned int n = 0;

    while(!bit(m_iPos + n))
        n++;

    m_iPos += n + 1;

//    int res = (int)pow(2.0, (double)n) - 1 + readBits(n);

    return (int) (pow(-1, n+1)*ceil((double)n/2));
}

/**
 * Reads n bits from the bitstream and returns them as an
 * unsigned integer.
 *
 * \param n Count of bits to read.
 * \return Unsigned integer computed out of the bits.
 */
unsigned int NALPayLoadStream::readBits(unsigned int n) {

    bool bits[n];

    unsigned int x = 0;

    for (unsigned int i = m_iPos; i < m_iPos + n; i++) bits[x++] = bit(i);

    m_iPos += n;

    return BitNumValue(bits, x);
}

unsigned int NALPayLoadStream::nextBits(unsigned int n)
{
    bool bits[n];

    unsigned int x = 0;

    for (unsigned int i = m_iPos; i < m_iPos + n; i++) bits[x++] = bit(i);

    return BitNumValue(bits, x);
}

/**
 * Converts a bit array to an unsigned integer.
 *
 * \param bits[] Array of bits.
 * \param size Size of bits[].
 * \return Unsigned integer representation of bits[].
 */
unsigned int NALPayLoadStream::BitNumValue(bool bits[], unsigned int size)
{
    int r = 0;
    int j = 0;

    for (int i = size - 1; i >= 0; i--) {

        r += bits[j] ? (unsigned int)pow(2, i) : 0;
        j++;
    }

    return r;
}

/**
 * Checks if more data is available in the payload.
 *
 * \return If more data available, TRUE.
 */
bool NALPayLoadStream::moreRbspData()
{
    if (m_pPayload->count() <= m_iPos)
        return false;
    else {

        unsigned int lastBit = m_iPos;

        for (unsigned int i = m_iPos; i < m_pPayload->count(); i++) {

            if (m_pPayload->test(i))
                lastBit = i;
        }

        if (lastBit > m_iPos)
            return true;
        else
            return false;
    }

}

inline
bool NALPayLoadStream::bit(unsigned int iPos)
{
	if (iPos >= m_pPayload->size())
		BOOST_THROW_EXCEPTION(H264::h264InvalidRbsp());
	else
		return (*m_pPayload)[iPos];
}

bool NALPayLoadStream::byte_aligned()
{
    if ((m_iPos % 8) == 0)
        return true;
    else
        return false;
}

bool NALPayLoadStream::moreRbspTrailingData()
{
    if (m_pPayload->count() <= m_iPos)
        return false;
    else
        return true;
}

};
