#include <inttypes.h>
#include <boost/tuple/tuple.hpp>
#include <boost/variant/variant.hpp>
#include <boost/smart_ptr.hpp>

#ifndef PARSER_TYPES_HPP_
#define PARSER_TYPES_HPP_

namespace H264Parser {

typedef boost::variant<std::string, int32_t> NalVar;
typedef boost::shared_ptr<NalVar> NalVarPtr;

/*
 * NAL unit types
 *
 */
enum nal_unit_type {

	NAL_T_UNSPECIFIED                           =  0, /* Unspecified                                */
	NAL_T_CODED_SLICE_OF_A_NON_IDR_PICTURE      =  1, /* Coded slice of a non-IDR picture           */
	NAL_T_SLICE_DATA_PARTITION_A_LAYER_RBSP     =  2, /* Coded slice data partition A               */
	NAL_T_SLICE_DATA_PARTITION_B_LAYER_RBSP     =  3, /* Coded slice data partition B               */
 	NAL_T_SLICE_DATA_PARTITION_C_LAYER_RBSP     =  4, /* Coded slice data partition C               */
 	NAL_T_CODED_SLICE_OF_AN_IDR_PICTURE         =  5, /* Coded slice of an IDR picture              */
 	NAL_T_SEI_RBSP                              =  6, /* Supplemental enhancement information (SEI) */
	NAL_T_SEQ_PARAMETER_SET_RBSP                =  7, /* Sequence parameter set                     */
	NAL_T_PIC_PARAMETER_SET_RBSP                =  8, /* Picture parameter set                      */
	NAL_T_ACCESS_UNIT_DELIMITER_RBSP            =  9, /* Access unit delimiter                      */
	NAL_T_END_OF_SEQ_RBSP                       = 10, /* End of sequence                            */
	NAL_T_END_OF_STREAM_RBSP                    = 11, /* End of stream                              */
	NAL_T_FILLER_DATA_RBSP                      = 12, /* Filler data                                */
	NAL_T_SEQ_PARAMETER_SET_EXTENSION_RBSP      = 13, /* Sequence parameter set extension           */
	NAL_T_PREFIX_NAL_UNIT_RBSP                  = 14, /* Prefix NAL unit                            */
	NAL_T_SUBSET_SEQ_PARAMETER_SET_RBSP         = 15, /* Subset sequence parameter set              */
	/* ... 16 - 18 RESERVED ... */
	NAL_T_CODED_SLICE_OF_AN_AUXILIARY_CODED_PIC = 19, /* Coded slice of an auxiliary coded picture without partitioning */
	NAL_T_SLICE_LAYER_EXTENSION_RBSP            = 20  /* Coded slice extension                      */
	/* ... 21 - 23 RESERVED ... */
	/* ... 24 - 31 UNSPECIFIED ... */
};

/*
 * NAL Header
 */
struct nal_hdr {

    /* nal_unit_type specifies the type of RBSP data structure contained
     * in the NAL unit as specified in Table 7-1. */
    uint32_t nal_unit_type      : 5;

    /* nal_ref_idc not equal to 0 specifies that the content of the NAL unit
     * contains a sequence parameter set, a sequence parameter set extension,
     * a subset sequence parameter set, a picture parameter set, a slice of a
     * reference picture, a slice data partition of a reference picture, or a
     * prefix NAL unit preceding a slice of a reference picture. */
    uint32_t nal_ref_idc        : 2;

    /* forbidden_zero_bit shall be equal to 0. */
    uint32_t forbidden_zero_bit : 1;
};

/*
 * Slice header
 * nal_unit_type: various
 * slice_header()
 */
struct slice_header {

    unsigned int first_mb_in_slice;
    unsigned int slice_type;
    unsigned int pic_parameter_set_id;

    unsigned int colour_plane_id : 2;
    unsigned int frame_num;
};

};

#endif /* PARSER_TYPES_HPP_ */
