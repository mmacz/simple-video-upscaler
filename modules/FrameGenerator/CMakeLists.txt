add_library(FrameGenerator STATIC
  src/FrameGenerator.cpp include/FrameGenerator.h
)
target_include_directories(FrameGenerator
  PUBLIC
    include
)
target_link_libraries(FrameGenerator
  PUBLIC
    opencv::opencv
)

