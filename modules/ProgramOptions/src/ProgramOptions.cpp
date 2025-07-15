#include "ProgramOptions.h"
#include "cxxopts.hpp"
#include <exception>
#include <iostream>

ProgramOptionsParser::ProgramOptionsParser(int argc, char **argv) {
  try {
    cxxopts::Options options(argv[0],
                             "Motion aware upscaler and frame generator");

    options.add_options()("i,input", "Input file path",
                          cxxopts::value<std::string>())(
        "o,output", "Output file path", cxxopts::value<std::string>())(
        "s,scale", "Scale factor",
        cxxopts::value<float>()->default_value("1.0"))("h,help", "Print usage");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    _options.inputFilePath = result["input"].as<std::string>();
    _options.outputFilePath = result["output"].as<std::string>();
    _options.scaleFactor = result["scale"].as<float>();

    if (_options.inputFilePath.empty() || _options.outputFilePath.empty()) {
      throw std::runtime_error(
          "Input and output file paths must be specified.");
    }
  } catch (const std::exception &e) {
    std::cerr << "Error parsing options: " << e.what() << std::endl;
    throw std::runtime_error("Invalid or missing options");
  }
}

ProgramOptionsParser::~ProgramOptionsParser() {
}

const ProgramOptions& ProgramOptionsParser::getOptions() const {
  return _options;
}
