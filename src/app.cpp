#include "ProgramOptions.h"
#include <iostream>

int main(int argc, char* argv[]) {
  try {
    ProgramOptionsParser parser(argc, argv);
    const auto opts = parser.getOptions();
  }
  catch (const std::runtime_error& e) {
    std::cerr << "Failed to parse options: " << e.what() << std::endl;
    return 1;
  }
}

