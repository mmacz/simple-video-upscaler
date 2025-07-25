#include "ProgramOptions.h"
#include "cxxopts.hpp"
#include <exception>
#include <iostream>

ProgramOptionsParser::ProgramOptionsParser() {
}

ProgramOptionsParser::~ProgramOptionsParser() {
}

const ProgramOptions ProgramOptionsParser::parse(int argc, char **argv) {
  try {
    cxxopts::Options options(argv[0],
                             "Motion aware upscaler and frame generator");

    options.add_options()
        ("i,input", "Input file path", cxxopts::value<std::string>())
        ("o,output", "Output file path", cxxopts::value<std::string>())
        ("in,insert", "Frames to insert", cxxopts::value<int32_t>()->default_value("2"))
        ("h,help", "Print usage");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    ProgramOptions po;
    po.inputFilePath = result["input"].as<std::string>();
    po.outputFilePath = result["output"].as<std::string>();
    po.insertCount = result["insert"].as<int32_t>();

    if (po.inputFilePath.empty() || po.outputFilePath.empty()) {
      throw std::runtime_error(
          "Input and output file paths must be specified.");
    }

    return po;
  } catch (const std::exception &e) {
    std::cerr << "Error parsing options: " << e.what() << std::endl;
    throw std::runtime_error("Invalid or missing options");
  }
}
