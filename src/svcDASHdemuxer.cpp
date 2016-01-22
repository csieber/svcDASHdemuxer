#include <iostream>
#include <boost/program_options.hpp>
#include "svcdemux.h"

int main(int argc, char** argv) {

	/*
	 * Read command line options.
	 */
	namespace po = boost::program_options;

	po::options_description desc("Options");
	desc.add_options()
	    ("help", "Help")
	    ("input,i", po::value<std::string>()->required(), "Input SVC stream")
		("output,o", po::value<std::string>()->default_value("segments"), "Output directory")
		("idr-per-segment,n", po::value<int32_t>()->default_value(1), "IDR Periods per segment. IDR periods are counted by NAL units with type NAL_T_SEQ_PARAMETER_SET_RBSP(7).")
		("filename-prefix,p", po::value<std::string>()->required(), "Filename suffix of the output segment files")
		("filename-suffix,s", po::value<std::string>()->default_value(".264"), "Filename prefix of the output segment files")
		("ifps,f", po::value<int32_t>()->default_value(24), "Intended frames per segment.");

	po::variables_map vm;

	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch (boost::program_options::required_option& ex) {

		std::cerr << "Required option '" << ex.get_option_name()
				  << "' is missing!" << std::endl << std::endl
				  << desc << std::endl;

		return 1;
	}
	catch (boost::program_options::invalid_option_value& ex) {

		std::cerr << "Option '" << ex.get_option_name()
				  << "' has an invalid value!" << std::endl << std::endl
				  << desc << std::endl;

		return 1;
	}
	catch (boost::program_options::unknown_option& ex) {

		std::cerr << "Unknown option '" << ex.get_option_name()
				  << "'!" << std::endl << std::endl
				  << desc << std::endl;

		return 1;
	}
	catch (boost::program_options::invalid_command_line_syntax& ex) {

		std::cerr << "Invalid syntax! What(): '" << ex.what() << std::endl << std::endl
				  << desc << std::endl;

		return 1;
	}

	if (vm.count("help")) {
	    std::cout << desc << "\n";
	    return 1;
	}

	std::string input_file  = vm["input"].as<std::string>();
	std::string output_dir 	= vm["output"].as<std::string>();

	std::string prefix 		= vm["filename-prefix"].as<std::string>();
	std::string suffix 		= vm["filename-suffix"].as<std::string>();
	int32_t idr_per_segment = vm["idr-per-segment"].as<int32_t>();
	int32_t ifps 			= vm["ifps"].as<int32_t>();

	return svcdemux(input_file, output_dir, prefix, suffix, idr_per_segment, ifps);
}
