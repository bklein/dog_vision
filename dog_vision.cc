#include <algorithm>
#include <iostream>
#include <numeric>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "mem.h"

using Buffer = std::vector<uint8_t>;

int main(int argc, char** argv) {

  cv::VideoCapture cap(0);
  if (!cap.isOpened())
    return -1;

  if (!cap.set(CV_CAP_PROP_FRAME_WIDTH, 640.0))
    return -1;

  if (!cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480.0))
    return -1;

  const size_t n_pixels = 640 * 480;
  cv::Mat rgb = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar::all(0));
  cv::Mat dog = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar::all(0));
  cv::Mat grey = cv::Mat(cv::Size(640, 480), CV_8UC1, cv::Scalar(0));

  double total_duration = 0.0;
  int count = 0;

  Buffer b(n_pixels);
  Buffer g(n_pixels);
  Buffer r(n_pixels);
  Buffer dog_r(n_pixels);

  std::vector<cv::Mat> channels;
  channels.push_back(cv::Mat(rgb.size(), CV_8UC1, cv::Scalar(0)));
  channels.push_back(cv::Mat(rgb.size(), CV_8UC1, cv::Scalar(0)));
  channels.push_back(cv::Mat(rgb.size(), CV_8UC1, cv::Scalar(0)));

  cv::namedWindow("original");
  cv::moveWindow("original", 0, 0);
  cv::namedWindow("dog");
  cv::moveWindow("dog", rgb.cols, 0);

  while (true) {

    cap >> rgb;

    auto t1 = cv::getTickCount();

    cv::split(rgb, channels);

    std::copy(channels.at(0).data, channels.at(0).data + channels.at(0).cols * channels.at(0).rows, b.begin());
    std::copy(channels.at(1).data, channels.at(1).data + channels.at(0).cols * channels.at(0).rows, g.begin());
    std::copy(channels.at(2).data, channels.at(2).data + channels.at(0).cols * channels.at(0).rows, r.begin());

    Buffer& dog_b = b;

    auto color_corrector =
      [] (const uint8_t g, const uint8_t r) -> uint8_t {
        return (static_cast<uint64_t>(g) + static_cast<uint64_t>(r)) / 2;
      };
    std::transform(g.begin(), g.end(), r.begin(), dog_r.begin(), color_corrector);
    Buffer& dog_g = dog_r;

    cv::cvtColor(rgb, grey, cv::COLOR_RGB2GRAY);
    double avg_brightness = cv::mean(grey)[0];

    auto averager =
      [avg_brightness] (const uint8_t n) -> uint8_t {
        return uint8_t((static_cast<double>(n) + avg_brightness) / 2.0);
      };

    std::transform(dog_b.begin(), dog_b.end(), dog_b.begin(), averager);
    std::transform(dog_g.begin(), dog_g.end(), dog_g.begin(), averager);
    std::transform(dog_r.begin(), dog_r.end(), dog_r.begin(), averager);

    for (size_t i = 0; i < dog.rows * dog.cols; ++i) {
      dog.at<cv::Vec3b>(i) = cv::Vec3b(dog_b[i], dog_g[i], dog_r[i]);
    }

    cv::GaussianBlur(dog, dog, cv::Size(3, 3), 0, 0);
    cv::GaussianBlur(dog, dog, cv::Size(5, 5), 0, 0);
    cv::GaussianBlur(dog, dog, cv::Size(7, 7), 0, 0);

    auto t2 = cv::getTickCount();

    auto duration = ((double)t2-t1) / cv::getTickFrequency();
    total_duration += duration;
    ++count;
    std::cout << "mean fps: " << 1.0 / (total_duration / static_cast<double>(count))
      << " mem usage: " << getCurrentRSS() / (1024 * 1024) << "mb peak: " << getPeakRSS() / (1024*1024) << "mb"
      << std::endl;

    cv::resize(rgb, rgb, cv::Size(640,480));
    cv::imshow("original", rgb);

    cv::resize(dog, dog, cv::Size(640, 480));
    cv::imshow("dog", dog);
    if (cv::waitKey(1) >= 0) break;

  }

  return 0;
}
