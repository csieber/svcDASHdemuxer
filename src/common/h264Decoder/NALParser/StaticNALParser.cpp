#include <stdio.h>
#include <string.h>
#include "types.hpp"
#include "../types.hpp"
#include "../h264Error.hpp"
#include "NALPayloadStream.h"
#include "StaticNALParser.h"

namespace H264Parser {
using namespace H264;

StaticNALParser::StaticNALParser() {
}

StaticNALParser::~StaticNALParser() {
}

nal_hdr StaticNALParser::parseNalHdr(raw_data nal_ptr)
{
	if (nal_ptr.get<1>() < 1)
		BOOST_THROW_EXCEPTION(h264InvalidNal() << errinfo_message("NAL unit too small!"));

	nal_hdr hdr;

	memcpy(&hdr, &nal_ptr.get<0>()[0], 1);

	return hdr;
}

/*
 * Extracts the Rbsp data (i.e. payload) from the NAL unit.
 *
 * @var nal_ptr Raw NAL unit (does NOT include the 4 bytes separator)
 * @var hdr_len Length of the header. 1 for H264, 4 for H264/SVC!
 * @return Returns the raw rbsp data of that NAL unit. NULL if there
 *         is no rbsp data.
 */
raw_data StaticNALParser::extractRbspData(raw_data nal_ptr, int32_t hdr_len)
{
	int32_t bytesInNalUnit = nal_ptr.get<1>();
	int32_t numBytesInRBSP = 0;

	const char* nal = nal_ptr.get<0>().get();

	if ((bytesInNalUnit - hdr_len) == 0)
		return raw_data(boost::shared_array<char>(), 0);

    if (bytesInNalUnit - hdr_len < 0)
    	BOOST_THROW_EXCEPTION(h264InvalidNal() << errinfo_message("NAL payload <= 0!"));

    char* rbsp_byte = new char[bytesInNalUnit - hdr_len];

    for (int i = hdr_len; i < bytesInNalUnit; i++) {

    	if (i + 2 < bytesInNalUnit && /*&& next_bits( 24 ) == 0x000003 */
    			nal[i]     == 0x00  &&
    			nal[i + 1] == 0x00  &&
    			nal[i + 2] == 0x03 ) {

    		rbsp_byte[numBytesInRBSP++] = nal[i];
            rbsp_byte[numBytesInRBSP++] = nal[i + 1];

            i += 2;

        } else
        	rbsp_byte[numBytesInRBSP++] = nal[i];
    }

    return raw_data(boost::shared_array<char>(rbsp_byte), bytesInNalUnit - hdr_len);
}

slice_header StaticNALParser::parseSliceHeader(raw_data rbsp_ptr)
{
    NALPayLoadStream stream(rbsp_ptr);

    slice_header hdr;

    hdr.first_mb_in_slice    = stream.readUE();
    hdr.slice_type           = stream.readUE();
    hdr.pic_parameter_set_id = stream.readUE();

    /* ... colour_plane_id ... */

//    if (pSPS == NULL) {

//        printf("WARNING: SVCSliceHeaderInScalableExt needs a decoded SeqParameterSet first! Using default value.\n");
    hdr.frame_num = stream.readBits(5 + 4);
/*
    }
    else {

        raw.frame_num = stream.readBits(pSPS->raw.log2_max_frame_num_minus4 + 4);
    }
*/
    return hdr;
}

}
