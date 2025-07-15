#include "FrameGenerator.h"
#include "ProgramOptions.h"
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <optional>

static void log_progress(int input_frames, int output_frames,
                         std::optional<int> total_input = std::nullopt,
                         int log_interval = 10,
                         std::chrono::steady_clock::time_point start_time =
                             std::chrono::steady_clock::now()) {
  if (input_frames % log_interval != 0 && !total_input.has_value())
    return;

  using namespace std::chrono;
  auto now = steady_clock::now();
  auto elapsed = duration_cast<seconds>(now - start_time).count();

  if (total_input.has_value()) {
    int percent =
        static_cast<int>((input_frames * 100.0f) / total_input.value());
    std::cout << "\r[" << percent << "%] " << input_frames << "/"
              << total_input.value() << " input, ~" << output_frames
              << " output frames"
              << " | elapsed: " << elapsed << "s" << std::flush;
  } else {
    std::cout << "\r[INFO] Processed " << input_frames << " input, wrote ~"
              << output_frames << " | elapsed: " << elapsed << "s"
              << std::flush;
  }
}

int main(int argc, char *argv[]) {
  try {
    ProgramOptionsParser parser;
    const auto opts = parser.parse(argc, argv);

    if (opts.insertCount < 0) {
      std::cerr << "Insert count must be non-negative." << std::endl;
      return 1;
    }

    cv::VideoCapture cap(opts.inputFilePath);
    if (!cap.isOpened()) {
      std::cerr << "Error opening video file: " << opts.inputFilePath
                << std::endl;
      return 1;
    }

    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps_in = cap.get(cv::CAP_PROP_FPS);
    double fps_out = fps_in * (opts.insertCount + 1);

    int input_count = 1;
    int output_count = 0;
    int total_input = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));

    double duration_sec = (fps_in > 0) ? (total_input / fps_in) : 0.0;
    int minutes = static_cast<int>(duration_sec) / 60;
    int seconds = static_cast<int>(duration_sec) % 60;

    std::cout << "[INFO] Input video: " << total_input << " frames at "
              << fps_in << " FPS (" << minutes << "m " << seconds << "s)"
              << std::endl;

    cv::VideoWriter writer(opts.outputFilePath,
                           cv::VideoWriter::fourcc('a', 'v', 'c', '1'),
                           fps_out,
                           cv::Size(width, height));
    if (!writer.isOpened()) {
      std::cerr << "Failed to open output file for writing: "
                << opts.outputFilePath << std::endl;
      return 1;
    }

    FrameGenerator generator(opts.insertCount);
    cv::Mat prev, curr;

    if (!cap.read(prev)) {
      std::cerr << "Wideo jest puste." << std::endl;
      return 1;
    }

    auto start_time = std::chrono::steady_clock::now();

    while (true) {
      if (!cap.read(curr)) {
        writer.write(prev);
        ++output_count;
        break;
      }

      writer.write(prev);
      ++output_count;

      auto interpolated = generator.generate(prev, curr);
      for (const auto &frame : interpolated) {
        writer.write(frame);
        ++output_count;
      }

      ++input_count;
      log_progress(input_count, output_count, total_input, 10, start_time);

      prev = curr.clone();
    }

    writer.write(prev);
    ++output_count;
    log_progress(input_count, output_count, total_input, 1, start_time);
    std::cout << std::endl
              << "[DONE] Wrote " << output_count << " frames from "
              << input_count << " input frames in "
              << std::chrono::duration_cast<std::chrono::seconds>(
                     std::chrono::steady_clock::now() - start_time)
                     .count()
              << " seconds." << std::endl;

    cap.release();
    writer.release();

    return 0;
  } catch (const std::runtime_error &e) {
    std::cerr << "Failed to parse options: " << e.what() << std::endl;
    return 1;
  }
}
