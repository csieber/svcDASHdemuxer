#include "../types.hpp"

#ifndef TYPES_SVC_HPP_
#define TYPES_SVC_HPP_

namespace H264Parser {

/*
 * Prefix NAL unit, Coded slice extension
 * nal_unit_type: 14 || 20
 */
struct nal_unit_header_extension_annexg {

    /* reserved_three_2bits shall be equal to 3. Other values of reserved_three_2bits may be specified in the
     * future by ITU-T | ISO/IEC. Decoders shall ignore the value of reserved_three_2bits. */
    uint32_t reserved_three_2bits     : 2;

    /* output_flag affects the decoded picture output and removal processes as specified in Annex C. The value
     * of output_flag shall be the same for all NAL units of a dependency representation. For any particular
     * value of dependency_id, the value of output_flag shall be the same for both fields of a complementary
     * field pair. */
    uint32_t output_flag              : 1;

    /* discardable_flag equal to 1 specifies that the current NAL unit is not used for decoding dependency
     * representations that are part of the current coded picture or any subsequent coded picture in decoding
     * order and have a greater value of dependency_id than the current NAL unit. discardable_flag equal to 0
     * specifies that the current NAL unit may be used for decoding dependency representations that are part
     * of the current coded picture or any subsequent coded picture in decoding order and have a greater value
     * of dependency_id than the current NAL unit. */
    uint32_t discardable_flag         : 1;

    /* use_ref_base_pic_flag equal to 1 specifies that reference base pictures (when present) and decoded
     * pictures (when reference base pictures are not present) are used as reference pictures for inter
     * prediction as specified in subclause G.8.2.3. use_ref_base_pic_flag equal to 0 specifies that
     * reference base pictures are not used as reference pictures for inter prediction (i.e., only decoded
     * pictures are used for inter prediction).
     *
     * The values of use_ref_base_pic_flag shall be the same for all NAL units of a dependency representation. */
    uint32_t use_ref_base_pic_flag    : 1;

    /* temporal_id specifies a temporal identifier for the NAL unit. The assignment of values to
     * temporal_id is constrained by the sub-bitstream extraction process as specified in subclause G.8.8.1.
     *
     * The value of temporal_id shall be the same for all prefix NAL units and coded slice in scalable
     * extension NAL units of an access unit. When an access unit contains any NAL unit with nal_unit_type
     * equal to 5 or idr_flag equal to 1, temporal_id shall be equal to 0. */
    uint32_t temporal_id              : 3;

    /* quality_id specifies a quality identifier for the NAL unit. quality_id shall be equal to 0
     * in prefix NAL units. The assignment of values to quality_id is constrained by the sub-bitstream
     * extraction process as specified in subclause G.8.8.1. */
    uint32_t quality_id               : 4;

    /* dependency_id specifies a dependency identifier for the NAL unit. dependency_id shall
     * be equal to 0 in prefix NAL units. The assignment of values to dependency_id is constrained
     * by the sub-bitstream extraction process as specified in subclause G.8.8.1. */
    uint32_t dependency_id            : 3;

    /* no_inter_layer_pred_flag specifies whether inter-layer prediction may be used
     * for decoding the coded slice. When no_inter_layer_pred_flag is equal to 1,
     * inter-layer prediction is not used for decoding the coded slice. When
     * no_inter_layer_pred_flag is equal to 0, inter-layer prediction may be used for
     * decoding the coded slice as signalled in the macroblock layer.
     *
     * For prefix NAL units, no_inter_layer_pred_flag shall be equal to 1. When nal_unit_type
     * is equal to 20 and quality_id is greater than 0, no_inter_layer_pred_flag shall be equal to 0.
     *
     * The variable MinNoInterLayerPredFlag is set equal to the minimum value of no_inter_layer_pred_flag
     * for the slices of the layer representation. */
    uint32_t no_inter_layer_pred_flag : 1;

    /* priority_id specifies a priority identifier for the NAL unit. The assignment
     * of values to priority_id is constrained by the sub-bitstream extraction process
     * as specified in subclause G.8.8.1. */
    uint32_t priority_id              : 6;

    /* idr_flag equal to 1 specifies that the current coded picture is an IDR
     * picture when the value of dependency_id for the NAL unit is equal to the
     * maximum value of dependency_id in the coded picture. idr_flag equal to 0
     * specifies that the current coded picture is not an IDR picture when the
     * value of dependency_id for the NAL unit is equal to the maximum value of
     * dependency_id in the coded picture. The value of idr_flag shall be the
     * same for all NAL units of a dependency representation. */
    uint32_t idr_flag                 : 1;
    uint32_t svc_extension_flag       : 1;

    // The (smallest) datatype uint32_t forces this struct to be 32 bits long
    // so we add those 8 bits to clarify that fact.
    uint32_t : 8;
};

typedef nal_unit_header_extension_annexg nal_hdr_svc;

/*
 * Coded Slice Extension
 * nal_unit_type: 20
 */
struct slice_header_in_scalable_extension {

    unsigned int first_mb_in_slice;
    unsigned int slice_type;
    unsigned int pic_parameter_set_id;
    unsigned int colour_plane_id: 2;
    unsigned int frame_num;
};

typedef slice_header_in_scalable_extension slice_header_svc;

};

#endif /* TYPES_SVC_HPP_ */
