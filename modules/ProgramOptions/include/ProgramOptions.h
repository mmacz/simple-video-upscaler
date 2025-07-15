#pragma once

#include <string>
#include <cstdint>

struct ProgramOptions {
  std::string inputFilePath;
  std::string outputFilePath;
  int32_t insertCount;
};

class ProgramOptionsParser {
public:
    ProgramOptionsParser();
    ~ProgramOptionsParser();

    const ProgramOptions parse(int argc, char **argv);
};

