#ifndef NAL_HEADER_HPP
#define NAL_HEADER_HPP

#include <inttypes.h>
#include <boost/static_assert.hpp>

namespace H264 {

/*
 * The mps VN (virtual network) NAL header is added as a prefix to
 * every packet send in our multi-path streaming concept. The length
 * of this header is always 12 Bytes.
 */
struct mps_nal_header {

	/*
     * Frame
     *
     * Range: 2^24 = 0 - 16777216
     * For 60 frames/sec this equals
     * 279620 seconds, 77 hours.
    */
	uint32_t frame_num : 24;
	
	/*
	 * The flow the packet belongs to.
	 */
	uint8_t flow_id;

	/*
	 * Number of packets used to transmit
	 * the NAL unit and the current number
	 * of the current packet.
	 *
	 * Range: 2^16 = 0 - 65536
	 * With a packet size of 1000 bytes this
	 * equals 65536 Kilobyte NAL units.
	 */
	uint16_t split_n;
	uint16_t split_c;

	/*
	* NAL unit type. Equals definition
	* in the H.264/SVC specification.
	*/
	uint8_t nal_unit_type;

	/*
	 * NAL unit layer. Equals definition
	 * in the H.264/SVC specification.
	 */
	uint8_t dependency_id;
	uint8_t temporal_id;
	uint8_t quality_id;
};

const int32_t mps_nal_header_len = sizeof(mps_nal_header);

/*
 * We want our MPS NAL Header to always compile to a size of 12 Bytes.
 * NOTE:
 *  If this assertion fails, the compiler throws something like 'incomplete type boost::STATIC_ASSERTION_FAILURE<false>'.
 */
BOOST_STATIC_ASSERT(mps_nal_header_len == 12);

}; // namespace H264

#endif // NAL_HEADER_HPP
