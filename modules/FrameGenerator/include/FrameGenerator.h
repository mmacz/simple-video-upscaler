#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class FrameGenerator {
public:
  FrameGenerator(int insertCount = 2);
    ~FrameGenerator();

    std::vector<cv::Mat> generate( const cv::Mat& prev, const cv::Mat& next);

private:
  int _insertCount;
  cv::Mat interpolate( const cv::Mat& prev, const cv::Mat& next, float t);
};
