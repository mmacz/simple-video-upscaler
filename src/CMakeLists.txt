add_executable(pixel_vectors
    app.cpp
)
target_include_directories(pixel_vectors
  PRIVATE
    src
)
target_link_libraries(pixel_vectors
  PRIVATE
    ProgramOptions
    FrameGenerator
    opencv::opencv
)

