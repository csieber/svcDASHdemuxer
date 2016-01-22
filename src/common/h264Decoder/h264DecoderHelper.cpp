#ifndef H264_DECODERHELPER
#define H264_DECODERHELPER

#include <iostream>
#include <list>
#include <boost/foreach.hpp>
#include "types.hpp"

namespace H264 {

/**
 * The function fills in layer and frame informations in NAL units who
 * are not carrying those informations themself. All units have to belong
 * to one frame.
 */
static void fill_blank_nal_units_in_frame(std::list<nal_short_info>& nal_list) {

	int32_t frame = -1;

	BOOST_FOREACH(std::list<nal_short_info>::value_type& nal, nal_list) {

		if (nal.frame_num != -1) {

			if (frame != -1 && nal.frame_num != frame) {

				std::cerr << "fill_blank_nal_units_in_frame only handles ONE frame at a time!!" << std::endl;
				return;
			}

			frame = nal.frame_num;
		}
	}

	int32_t dependency = 0;
	int32_t temporal = 0;
	int32_t quality = 0;

	BOOST_FOREACH(std::list<nal_short_info>::value_type& nal, nal_list) {

		nal.frame_num = frame;

		if (nal.type == 14) {

			dependency = nal.dependency_id;
			temporal = nal.temporal_id;
			quality = nal.quality_id;

		} else if (nal.type == 1) {

			nal.dependency_id = dependency;
			nal.temporal_id = temporal;
			nal.quality_id = quality;

		} else {

			if (nal.dependency_id == -1) {

				nal.dependency_id = 0;
				nal.temporal_id = 0;
				nal.quality_id = 0;
			}

			dependency = 0;
			temporal = 0;
			quality = 0;
		}
	}
};





}; // namespace H264

#endif // H264_DECODERHELPER
