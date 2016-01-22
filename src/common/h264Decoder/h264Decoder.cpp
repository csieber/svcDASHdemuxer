#include "types.hpp"
#include "NALParser/SVC/StaticNALParserSVC.h"
#include "NALParser/StaticNALParser.h"
#include "h264Decoder.h"

namespace H264 {

h264Decoder::h264Decoder(NALStream* pStream) : m_pStream(pStream){
	frame_num_counter = -1;
}

h264Decoder::~h264Decoder() {
}

nal_short_info h264Decoder::next()
{
	raw_data nal = m_pStream->next();

	if (nal.get<1>() == -1)
		return nal_short_info();
	else
		return parseNAL(nal);
}

nal_short_info h264Decoder::parseNAL(raw_data nal)
{
	nal_short_info nal_info;
	H264Parser::nal_hdr hdr;

	hdr = H264Parser::StaticNALParser::parseNalHdr(nal);

	nal_info << hdr;
	nal_info.nal_nr = m_pStream->posNal() - 1;
	nal_info.size = nal.get<1>();
	nal_info.pNal = nal;

	raw_data rbsp;

	if (H264Parser::StaticNALParserSVC::isSvcNal(hdr)) {

		H264Parser::nal_hdr_svc hdr_svc
			= H264Parser::StaticNALParserSVC::parseSvcHdr(nal, hdr);

		nal_info << hdr_svc;

	} else {

		rbsp = H264Parser::StaticNALParser::extractRbspData(nal, 1);
	}

	// Prefix NAL units do mark the beginning of a frame, but not in all cases.
	if (hdr.nal_unit_type == H264Parser::NAL_T_CODED_SLICE_OF_A_NON_IDR_PICTURE ||
	    hdr.nal_unit_type == H264Parser::NAL_T_CODED_SLICE_OF_AN_IDR_PICTURE) {

	   frame_num_counter++;
	}

	if (hdr.nal_unit_type == H264Parser::NAL_T_PREFIX_NAL_UNIT_RBSP) {
		last_prefix_nal_ = nal_info;
	}
	else if (hdr.nal_unit_type == H264Parser::NAL_T_SLICE_LAYER_EXTENSION_RBSP) {

		last_prefix_nal_.dependency_id = nal_info.dependency_id;
		last_prefix_nal_.temporal_id = nal_info.temporal_id;
		last_prefix_nal_.quality_id = nal_info.quality_id;

	// Meta data NAL unit check.
	} else if (hdr.nal_unit_type == H264Parser::NAL_T_SEI_RBSP) {

		last_prefix_nal_.nal_nr = -1;
	}

	if (frame_num_counter == -1)
	  nal_info.frame_num = 0;
	else
	  nal_info.frame_num = frame_num_counter;

	return nal_info;
}

}
