#include <stdio.h>
#include <boost/foreach.hpp>
#include "NALParser/types.hpp"
#include "h264Error.hpp"
#include "h264StreamDb.h"

namespace H264 {

h264StreamDb::h264StreamDb()
{
	m_pFile = NULL;

	m_firstNalOfFrame[0] = 0;
	m_lastFrameNr = 0;
}

h264StreamDb::~h264StreamDb() {
}

const char hdr[] = "\
|    NAL | Type |   Size | Frame | ( D/ T/ Q) |\n\
|---------------------------------------------|\n\
";

const char row_read[] = "\
| %d |   %d | %d |  %d | (%d/%d/%d) |\n\
";

const char row_write[] = "\
| %6d |   %2d | %6d |  %5d | (%2d/%2d/%2d) |\n\
";

bool h264StreamDb::load(std::string sFile)
{
    m_pFile = fopen(sFile.c_str(), "rb");

    if (!m_pFile)
        return false;

    fscanf(m_pFile, hdr);

    nal_short_info info;

	while (	fscanf(m_pFile, row_read, &info.nal_nr
			  	 	    , &info.type
					    , &info.size
					    , &info.frame_num
					    , &info.dependency_id
					    , &info.temporal_id
					    , &info.quality_id) != EOF) {
//TODO error checking for corrupt db files?
		push_back(info);
	}

	fclose(m_pFile);

    m_sFile = sFile;

	return true;
}

void h264StreamDb::push_back(nal_short_info& info)
{
	if (m_firstNalOfFrame.count(info.frame_num) == 0)
		m_firstNalOfFrame[info.frame_num] = info.nal_nr;

	m_nalDb.push_back(info);
}

bool h264StreamDb::save(std::string sFile)
{
	m_pFile = fopen(sFile.c_str(), "w");

	if (!m_pFile)
		return false;

	fprintf(m_pFile, hdr);

	BOOST_FOREACH(nal_short_info info, m_nalDb) {

		fprintf(m_pFile, row_write, info.nal_nr
							, info.type
							, info.size
							, info.frame_num
							, info.dependency_id
							, info.temporal_id
							, info.quality_id);
	}

	fclose(m_pFile);

	return true;
}

std::list<nal_short_info> h264StreamDb::frame(const int32_t& frame_nr)
{
	if (frame_nr >= (int32_t)m_firstNalOfFrame.size())
		BOOST_THROW_EXCEPTION(h264RequestedFrameNotAvailable());

	std::list<nal_short_info> nal_list;

	int32_t end = 0;

	if (frame_nr + 1 >= (int32_t)m_firstNalOfFrame.size())
		end = (int32_t)m_nalDb.size();
	else
		end = m_firstNalOfFrame[frame_nr + 1];

	for (int32_t i = m_firstNalOfFrame[frame_nr]; i < end; i++)
		nal_list.push_back(m_nalDb[i]);

	return nal_list;
}

int32_t h264StreamDb::packetCountForPayloadSize(int32_t max_payload_size)
{
	int32_t count = 0;
	int32_t packets = 0;

	nal_short_info last_nal;

	BOOST_FOREACH(std::vector<nal_short_info>::value_type& nal, m_nalDb) {

		// Prefix NAL units are combined with the following NAL of the two types.
		if (nal.type == H264Parser::NAL_T_PREFIX_NAL_UNIT_RBSP)
			continue;

		if (last_nal.type          == nal.type &&
			last_nal.frame_num     == nal.frame_num &&
			last_nal.dependency_id == nal.dependency_id &&
			last_nal.temporal_id   == nal.temporal_id &&
			last_nal.quality_id    == nal.quality_id) {

			// that works only approximately..and assumes, that the second
			// of the combined NAL units fits into the first packet.
			packets = (int32_t)((4 + nal.size) / max_payload_size);

		} else 	if (nal.type == H264Parser::NAL_T_CODED_SLICE_OF_AN_IDR_PICTURE
			|| nal.type == H264Parser::NAL_T_CODED_SLICE_OF_A_NON_IDR_PICTURE) {

			// +9 because of the 5 bytes prefix unit and 4 bytes NAL separator.
			packets = (int32_t)((nal.size + 9) / max_payload_size) + 1;
		}
		else
			packets = (int32_t)(nal.size / max_payload_size) + 1;

		count += packets;

		last_nal = nal;
	}

	return count;
}

const nal_short_info& h264StreamDb::operator[](int32_t nal_nr)
{
	if (nal_nr >= (int32_t)m_nalDb.size())
		BOOST_THROW_EXCEPTION(h264RequestedNALNotAvailable());
	else
		return m_nalDb[nal_nr];
}

h264StreamDb& h264StreamDb::operator<<(nal_short_info info)
{
	info.pNal.get<0>() = boost::shared_array<char>();
	info.pNal.get<1>() = -1;

	push_back(info);

	return *this;
}

void h264StreamDb::reset()
{
	m_nalDb.clear();
}


}
