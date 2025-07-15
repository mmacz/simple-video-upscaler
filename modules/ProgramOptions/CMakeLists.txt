add_library(ProgramOptions
    src/ProgramOptions.cpp include/ProgramOptions.h
)
target_include_directories(ProgramOptions
  PUBLIC
    include
)
target_link_libraries(ProgramOptions
  PUBLIC
    cxxopts::cxxopts
)

