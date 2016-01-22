#include <stdio.h>
#include <string.h>
#include "h264SvcError.hpp"
#include "../NALPayloadStream.h"
#include "types_svc.hpp"
#include "StaticNALParserSVC.h"

namespace H264Parser {
using namespace H264;

StaticNALParserSVC::StaticNALParserSVC() {
}

StaticNALParserSVC::~StaticNALParserSVC() {
}

nal_hdr_svc StaticNALParserSVC::parseSvcHdr(raw_data nal_ptr, nal_hdr hdr)
{
    if (hdr.nal_unit_type != (uint32_t)NAL_T_PREFIX_NAL_UNIT_RBSP
     && hdr.nal_unit_type != (uint32_t)NAL_T_SLICE_LAYER_EXTENSION_RBSP)
    	BOOST_THROW_EXCEPTION(h264SvcInvalidNal() << errinfo_message("Tried to SVC-parse non-SVC unit!"));

    if (nal_ptr.get<1>() < 4)
    	BOOST_THROW_EXCEPTION(h264SvcInvalidNal() << errinfo_message("NAL unit too small to be a SVC unit!"));

    char buff[4];

    const char* data = nal_ptr.get<0>().get();

//TODO: find out why i did that here
    buff[0] = data[3];
    buff[1] = data[2];
    buff[2] = data[1];
    buff[3] = '\0';

    nal_hdr_svc hdr_svc;
	memcpy(&hdr_svc, buff, 4);

	return hdr_svc;
}

bool StaticNALParserSVC::isSvcNal(nal_hdr hdr)
{
    if (hdr.nal_unit_type == NAL_T_PREFIX_NAL_UNIT_RBSP
     || hdr.nal_unit_type == NAL_T_SLICE_LAYER_EXTENSION_RBSP) {
    	return true;
    } else
    	return false;
}

slice_header_svc StaticNALParserSVC::parseSliceHeader(raw_data rbsp_ptr)
{
    NALPayLoadStream stream(rbsp_ptr);

    slice_header_svc hdr;

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

};
