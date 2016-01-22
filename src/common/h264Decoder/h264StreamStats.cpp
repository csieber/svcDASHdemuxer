#define __STDC_FORMAT_MACROS

#include <stdio.h>
#include <inttypes.h>
#include <boost/foreach.hpp>
#include "h264StreamStats.h"

namespace H264 {

h264StreamStats::h264StreamStats() {

	m_iBytesTotal = 0;
	m_iNumbOfNals = 0;
	m_iNumbOfParsingErrors = 0;
	m_iNumbOfFrames = 0;
	m_iHighestFrameNr = -1;

	m_iLastFrameNr = -1;

	for (int i = 0; i < 3; i++) m_last_layer[i] = -1;
}

h264StreamStats::~h264StreamStats() {
}

h264StreamStats& h264StreamStats::operator<<(nal_short_info info)
{
	m_iBytesTotal += info.size;
	m_iNumbOfNals++;

	//NOTE: Compensate for appended streams with
	//      none consistent frame tagging.
	if (info.frame_num != -1 && info.frame_num != m_iLastFrameNr) {

		m_iNumbOfFrames++;
		m_iLastFrameNr = info.frame_num;

		if (m_iHighestFrameNr < info.frame_num) m_iHighestFrameNr = info.frame_num;
	}

	if (info.type == H264Parser::NAL_T_PREFIX_NAL_UNIT_RBSP) {

		m_last_layer[0] = info.temporal_id;
		m_last_layer[1] = info.quality_id;
		m_last_layer[2] = info.dependency_id;

	} else {

		if (info.type == H264Parser::NAL_T_CODED_SLICE_OF_A_NON_IDR_PICTURE) {

			info.temporal_id = m_last_layer[0];
			info.quality_id = m_last_layer[1];
			info.dependency_id = m_last_layer[2];
		}

		for (int i = 0; i < 3; i++) m_last_layer[i] = -1;
	}

	BOOST_FOREACH(h264_layer& layer, m_layers) {

		//Note: Operator== compares T/D/Q
		if (layer == info) {

			layer.numb_of_bytes += info.size;
			layer.numb_of_nals++;

			return *this;
		}
	}

	h264_layer new_lay;

	//Note: Operator= transfers T/D/Q
	new_lay = info;

	new_lay.numb_of_bytes += info.size;
	new_lay.numb_of_nals++;

	m_layers.push_back(new_lay);

	return *this;
}

const char hdr_template[] = "\
NALs: %%d\n\
Size: %%%s bytes\n\
Frames: %%d (highest frame number seen: %%d)\n\
Layers: %%d\n\
*------------------------------*\n\
| ( D/ T/ Q) | NALs |     Size |\n\
*------------------------------*\n";

const char row[] = "| (%2d/%2d/%2d) | %4d | %8d |\n";

bool h264StreamStats::save(std::string sFile)
{
	FILE* pFile = fopen(sFile.c_str(), "w");

	if (!pFile)
		return false;

	char fstrHdr[500];
	sprintf(fstrHdr, hdr_template, PRId64);

	fprintf(pFile, fstrHdr
			, m_iNumbOfNals, m_iBytesTotal
			, m_iNumbOfFrames, m_iHighestFrameNr, m_layers.size() - 1);

	BOOST_FOREACH(h264_layer& layer, m_layers) {

		fprintf(pFile, row
				, layer.dependency_id
				, layer.temporal_id
				, layer.quality_id
				, layer.numb_of_nals
				, layer.numb_of_bytes);
	}

	fprintf(pFile, "*------------------------------*\n");

	fclose(pFile);

	return true;
}

bool h264StreamStats::load(std::string sFile)
{
	FILE* pFile = fopen(sFile.c_str(), "rb");

	if (!pFile)
		return false;

	int32_t numb_of_layers = 0;

	char fstrHdr[500];
	sprintf(fstrHdr, hdr_template, PRId64);

	fscanf(pFile, fstrHdr
			, &m_iNumbOfNals
			, &m_iBytesTotal
			, &m_iNumbOfFrames
			, &m_iHighestFrameNr
			, &numb_of_layers);

	h264_layer layer;

	for (int i = 0; i <= numb_of_layers; i++) {

		fscanf(pFile, row
					, &layer.dependency_id
					, &layer.temporal_id
					, &layer.quality_id
					, &layer.numb_of_nals
					, &layer.numb_of_bytes);

		m_layers.push_back(layer);
	}

	fclose(pFile);

	return true;
}

void h264StreamStats::reset()
{
	m_layers.clear();
}

}
