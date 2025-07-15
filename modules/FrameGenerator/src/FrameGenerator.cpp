#include "FrameGenerator.h"

FrameGenerator::FrameGenerator(int insertCount)
    : _insertCount(insertCount)
{}

FrameGenerator::~FrameGenerator()
{}

std::vector<cv::Mat> FrameGenerator::generate(const cv::Mat& prev, const cv::Mat& next) {
    std::vector<cv::Mat> result;
    if (prev.empty() || next.empty() || _insertCount <= 0)
        return result;

    for (int i = 1; i <= _insertCount; ++i) {
        float t = static_cast<float>(i) / (_insertCount + 1);
        result.push_back(interpolate(prev, next, t));
    }
    return result;
}


cv::Mat FrameGenerator::interpolate(const cv::Mat& prev, const cv::Mat& next, float t) {
    cv::Mat g0, g1;
    cv::cvtColor(prev, g0, cv::COLOR_BGR2GRAY);
    cv::cvtColor(next, g1, cv::COLOR_BGR2GRAY);

    cv::Mat flow_fw, flow_bw;
    cv::calcOpticalFlowFarneback(g0, g1, flow_fw, 0.5, 3, 15, 3, 5, 1.2, 0);
    cv::calcOpticalFlowFarneback(g1, g0, flow_bw, 0.5, 3, 15, 3, 5, 1.2, 0);

    auto remapWithFlow = [](const cv::Mat& img, const cv::Mat& flow, float scale) {
        cv::Mat map_x(img.size(), CV_32FC1), map_y(img.size(), CV_32FC1);
        for (int y = 0; y < img.rows; ++y)
            for (int x = 0; x < img.cols; ++x) {
                const auto& f = flow.at<cv::Point2f>(y, x);
                map_x.at<float>(y, x) = x + f.x * scale;
                map_y.at<float>(y, x) = y + f.y * scale;
            }
        cv::Mat warped;
        cv::remap(img, warped, map_x, map_y, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
        return warped;
    };

    cv::Mat warped0 = remapWithFlow(prev, flow_fw, t);
    cv::Mat warped1 = remapWithFlow(next, flow_bw, -(1.0f - t));

    cv::Mat blended;
    cv::addWeighted(warped0, 1.0f - t, warped1, t, 0.0, blended);
    return blended;
}

