#include <algorithm>
#include <numeric>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dog_viz_lib.hpp>

using Buffer = std::vector<uint8_t>;

int main(int argc, char** argv) {

  //if (argc != 2) {
  //  std::cerr << "provide image filename" << std::endl;
  //  return 1;
  //}

  //const std::string filename(argv[1]);

  //cv::Mat rgb = cv::imread(filename);

  //if (rgb.empty()) {
  //  std::cerr << "empty image";
  //  return 1;
  //}
  //
  cv::VideoCapture cap(0);
  if (!cap.isOpened())
    return -1;

  if (!cap.set(CV_CAP_PROP_FRAME_WIDTH, 640.0))
    return -1;

  if (!cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480.0))
    return -1;

  const size_t n_pixels = 640 * 480;
  cv::Mat rgb;

  double total_duration = 0.0;
  int count = 0;

  Buffer b(n_pixels);
  Buffer g(n_pixels);
  Buffer r(n_pixels);
  Buffer dog_r(n_pixels);

  while (true) {

    cap >> rgb;

    auto t1 = cv::getTickCount();

    //std::cout << "splitting\n";
    std::vector<cv::Mat> channels(3);
    cv::split(rgb, channels);

    //auto b = Buffer(channels.at(0).data, channels.at(0).data + channels.at(0).cols * channels.at(0).rows);
    //auto g = Buffer(channels.at(1).data, channels.at(1).data + channels.at(0).cols * channels.at(0).rows);
    //auto r = Buffer(channels.at(2).data, channels.at(2).data + channels.at(0).cols * channels.at(0).rows);
    std::copy(channels.at(0).data, channels.at(0).data + channels.at(0).cols * channels.at(0).rows, b.begin());
    std::copy(channels.at(1).data, channels.at(1).data + channels.at(0).cols * channels.at(0).rows, g.begin());
    std::copy(channels.at(2).data, channels.at(2).data + channels.at(0).cols * channels.at(0).rows, r.begin());


    //Buffer dog_b(n_pixels);
    Buffer& dog_b = b;
    //Buffer dog_g(n_pixels);
    //Buffer dog_r(n_pixels);

    //std::cout << "applying color correction\n";
    auto color_corrector =
      [] (const uint8_t g, const uint8_t r) -> uint8_t {
        return (static_cast<uint64_t>(g) + static_cast<uint64_t>(r)) / 2;
      };
    std::transform(g.begin(), g.end(), r.begin(), dog_r.begin(), color_corrector);
    //for (size_t i = 0; i < n_pixels; ++i) {
    //  //dog_b[i] = b[i];
    //  dog_r[i] = (g[i] + r[i]) / 2;
    //  //dog_g[i] = dog_r[i];
    //}
    Buffer& dog_g = dog_r;

    cv::Mat grey;
    cv::cvtColor(rgb, grey, cv::COLOR_RGB2GRAY);
    double avg_brightness = cv::mean(grey)[0];

    //std::cout << "avg_brightness: " << avg_brightness << std::endl;

    auto averager =
      [avg_brightness] (const uint8_t n) -> uint8_t {
        return uint8_t((static_cast<double>(n) + avg_brightness) / 2.0);
      };

    std::transform(dog_b.begin(), dog_b.end(), dog_b.begin(), averager);
    std::transform(dog_g.begin(), dog_g.end(), dog_g.begin(), averager);
    std::transform(dog_r.begin(), dog_r.end(), dog_r.begin(), averager);
    //for (size_t i = 0; i < n_pixels; ++i) {
    //  dog_b[i] = ((double)dog_b[i] + avg_brightness) / 2.0;
    //  dog_g[i] = ((double)dog_g[i] + avg_brightness) / 2.0;
    //  dog_r[i] = ((double)dog_r[i] + avg_brightness) / 2.0;
    //}

    //std::cout << "merging\n";
    cv::Mat dog = cv::Mat(rgb.size(), CV_8UC3, cv::Scalar::all(0));
    for (size_t i = 0; i < dog.rows * dog.cols; ++i) {
      dog.at<cv::Vec3b>(i) = cv::Vec3b(dog_b[i], dog_g[i], dog_r[i]);
    }

    cv::GaussianBlur(dog, dog, cv::Size(3, 3), 0, 0);
    cv::GaussianBlur(dog, dog, cv::Size(5, 5), 0, 0);
    cv::GaussianBlur(dog, dog, cv::Size(7, 7), 0, 0);
    //cv::blur(dog, dog, cv::Size(2, 2));
    //cv::blur(dog, dog, cv::Size(4, 4));
    //cv::blur(dog, dog, cv::Size(6, 6));

    auto t2 = cv::getTickCount();

    auto duration = ((double)t2-t1) / cv::getTickFrequency();
    total_duration += duration;
    ++count;
    std::cout << "mean fps: " << 1.0 / (total_duration / static_cast<double>(count)) << std::endl;
    //std::cout << "took: " << duration << " secs " << 1.0 / duration <<  "fps\n";

    //cv::namedWindow("original");
    //cv::moveWindow("original", 0, 0);
    //cv::resize(rgb, rgb, cv::Size(640,480));
    //cv::imshow("original", rgb);

    //cv::namedWindow("dog");
    //cv::moveWindow("dog", rgb.cols, 0);
    //cv::resize(dog, dog, cv::Size(640, 480));
    //cv::imshow("dog", dog);
    //if (cv::waitKey(1) >= 0) break;

    //if (count > 90)
    //  break;
  }

  return 0;
}
