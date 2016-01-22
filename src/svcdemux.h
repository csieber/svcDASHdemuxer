#ifndef SVCDEMUX_H_
#define SVCDEMUX_H_

#include <string>
#include <inttypes.h>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include "common/h264Decoder/h264Decoder.h"
#include "common/h264Decoder/h264StreamDb.h"
#include "common/h264Decoder/h264StreamStats.h"
#include "common/h264Decoder/NALStreams/NALFileStream.h"

typedef boost::shared_ptr<std::ofstream> ofstream_ptr;

int svcdemux(std::string input,
		    std::string output_dir,
		    std::string file_suffix,
		    std::string file_prefix,
		    int32_t idr_per_segment,
		    int32_t ifps);

ofstream_ptr new_outstream(std::string output_dir,
		 	               std::string file_prefix,
		   				   std::string file_suffix,
		   				   int32_t layer,
						   int32_t segment);

void write_nal(ofstream_ptr stream, raw_data nal);

#endif /* SVCDEMUX_H_ */
