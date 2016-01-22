#include "svcdemux.h"
#include <iostream>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

int svcdemux(std::string input,
		    std::string output_dir,
		    std::string file_prefix,
		    std::string file_suffix,
		    int32_t idr_per_segment,
		    int32_t ifps)
{

	/*
	 * Read SVC file
	 */
	using H264::NALFileStream;
	using H264::h264Decoder;
	using H264::h264StreamDb;
	using H264::h264StreamStats;
	using H264::nal_short_info;

	NALFileStream* stream = new NALFileStream();

	if (!stream->open(input)) {

		std::cerr << "h264: Failed to open " << input << ". Exiting!" << std::endl;
		return 1;
	}

	if (!stream->loadIndex()) {

		std::cout << "h264: file index missing. Creating new one. This might take a while." << std::endl;

		if (!stream->createIndex()) {

			std::cerr << "h264: Failed to create index! Exiting!" << std::endl;
			return 1;
		}
	}

	// Decode the SVC file.
	H264::h264Decoder::ptr decoder = h264Decoder::ptr(new h264Decoder(stream));;

	nal_short_info nal = decoder->next();
	float perc = 0;

	// LayerID to segment stream
	std::map<int32_t, ofstream_ptr> layer_seg_streams;

	int32_t segment_counter = 0;
	int32_t idr_counter 	= -1;
	int32_t frame_counter	= 0;
	int32_t last_frame 		= 0;

	/*
	 * Parse through the whole bitstream,
	 */
	while (nal.pNal.get<1>() != -1) {

		if (stream->posNal() % 800 == 0) {

			perc = (float)stream->posNal()/(float)stream->nalIndexSize()*100;
			std::cout << "Progress: " << perc << "%" << std::endl;
		}

		if (nal.type == H264Parser::NAL_T_CODED_SLICE_OF_AN_IDR_PICTURE)
			idr_counter++;

		if (last_frame != nal.frame_num)
			frame_counter++;

		if ((idr_counter % idr_per_segment) == 0 && idr_counter != 0) {

			if (frame_counter != ifps && frame_counter != 0) {

				std::cout << "WARNING: Segment " << segment_counter << " contains " << frame_counter
						<< " instead of " << ifps << "!" << std::endl;
			}

			layer_seg_streams.clear();
			segment_counter++;

			idr_counter   = 0;
			frame_counter = 0;
		}

		const int32_t layer_id = nal.dependency_id == -1 ? 0 : nal.dependency_id;

		if (layer_seg_streams.count(layer_id) == 0) {

			layer_seg_streams[layer_id]
			                  = new_outstream(output_dir,
			                		  	  	  file_prefix,
			                		  	  	  file_suffix,
			                		  	  	  layer_id,
			                		  	  	  segment_counter);
		}

		write_nal(layer_seg_streams[layer_id], nal.pNal);

		last_frame = nal.frame_num;

		nal = decoder->next();
	}

	if (frame_counter != ifps && frame_counter != 0) {

		std::cout << "WARNING: Segment " << segment_counter << " contains " << frame_counter
				<< " instead of " << ifps << "!" << std::endl;
	}

	return 0;
}

void write_nal(ofstream_ptr stream, raw_data nal_data)
{
	const char nal_sep[] = {0x0, 0x0, 0x0, 0x01};

	stream->write(nal_sep, sizeof(nal_sep));
	stream->write(nal_data.get<0>().get(), nal_data.get<1>());
}

ofstream_ptr new_outstream(std::string output_dir,
			               std::string file_prefix,
						   std::string file_suffix,
						   int32_t layer,
						   int32_t segment)
{
	std::ofstream* stream = new std::ofstream();

	const std::string filename = boost::lexical_cast<std::string>(
										boost::format("%s/%s%02d_%04d%s")
											% output_dir
											% file_prefix
											% layer
											% segment
											% file_suffix);

	stream->open(filename.c_str());

	if (!stream->is_open()) {

		std::cerr << "Fatal: Could not open file '" << filename << "' for writing!" << std::endl;
		return ofstream_ptr();
	}

	stream->rdbuf()->pubsetbuf(0, 0);

	return ofstream_ptr(stream);
}
