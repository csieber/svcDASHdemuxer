#include <inttypes.h>
#include <string.h>
#include <boost/shared_array.hpp>
#include "NALParser/types.hpp"
#include "NALParser/SVC/types_svc.hpp"

#ifndef TYPES_HPP_
#define TYPES_HPP_

typedef boost::tuple<boost::shared_array<char>, int32_t> raw_data;

namespace H264 {

struct nal_short_info {

	nal_short_info() {

		type   = -1,
		size   = -1,
		nal_nr = -1,
		dependency_id = -1,
		temporal_id = -1,
		quality_id = -1,
		frame_num = -1;
		pNal.get<1>() = -1;
		split_n = -1;
		split_c = -1;
	};

	int32_t type;
	int32_t size;
	int32_t nal_nr;
	int32_t dependency_id;
	int32_t temporal_id;
	int32_t quality_id;
	int32_t frame_num;

	raw_data pNal;

	int32_t split_n;
	int32_t split_c;

	nal_short_info& operator=(const nal_short_info& op) {

		this->type = op.type;
		this->size = op.size;
		this->nal_nr = op.nal_nr;
		this->dependency_id = op.dependency_id;
		this->temporal_id = op.temporal_id;
		this->quality_id = op.quality_id;
		this->frame_num = op.frame_num;
		this->pNal = op.pNal;
		this->split_c = op.split_c;
		this->split_n = op.split_n;

		return *this;
	}

	nal_short_info& operator<<(H264Parser::nal_hdr& hdr) {

		type = (int32_t)hdr.nal_unit_type;

		return *this;
	};

	nal_short_info& operator<<(H264Parser::nal_hdr_svc& hdr) {

		dependency_id = (int32_t)hdr.dependency_id;
		temporal_id = (int32_t)hdr.temporal_id;
		quality_id = (int32_t)hdr.quality_id;

		return *this;
	};

	bool operator<(nal_short_info& hdr) {

		if (this->dependency_id < hdr.dependency_id)
			return true;
		if (this->dependency_id > hdr.dependency_id)
			return false;

		if (this->temporal_id < hdr.temporal_id)
			return true;
		if (this->temporal_id > hdr.temporal_id)
			return false;

		if (this->quality_id < hdr.quality_id)
			return true;
		if (this->quality_id > hdr.quality_id)
			return false;

		if (this->split_n < hdr.split_n)
			return true;

		return false;
	}

	bool no_layer() {
		if (dependency_id == -1 &&
			temporal_id == -1 &&
			quality_id == -1)
			return true;
		else
			return false;
	}
};

};

#endif /* TYPES_HPP_ */
