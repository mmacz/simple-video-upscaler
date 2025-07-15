#pragma once

#include <string>

struct ProgramOptions {
  std::string inputFilePath;
  std::string outputFilePath;
  float scaleFactor;
};

class ProgramOptionsParser {
public:
    ProgramOptionsParser();
    ~ProgramOptionsParser();

    const ProgramOptions parse(int argc, char **argv);
};

