#pragma once

#include <string>

struct ProgramOptions {
  std::string inputFilePath;
  std::string outputFilePath;
  float scaleFactor;
};

class ProgramOptionsParser {
public:
    ProgramOptionsParser(int argc, char** argv);
    ~ProgramOptionsParser();

    const ProgramOptions& getOptions() const;
private:
  ProgramOptions _options;
};

