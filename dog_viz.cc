#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dog_viz_lib.hpp>

int main(int argc, char** argv) {

  if (argc != 2) {
    std::cerr << "provide image filename" << std::endl;
    return 1;
  }

  const std::string filename(argv[1]);

  cv::Mat rgb = cv::imread(filename);

  if (rgb.empty()) {
    std::cerr << "empty image";
    return 1;
  }

  DogImage::Ptr dog_image = DogImage::create(rgb);
  if (!dog_image) {
    std::cerr << "image not 3 channel";
    return 1;
  }

  applyColorCorrection(*dog_image);
  // applyBrightnessCorrection(dog_image.get());
  // applyAcuityCorrection(dog_image.get());
  //
  cv::namedWindow("original");
  cv::moveWindow("original", 0, 0);
  cv::resize(rgb, rgb, cv::Size(640,480));
  cv::imshow("original", rgb);

  cv::namedWindow("dog");
  cv::moveWindow("dog", rgb.cols, 0);
  cv::Mat dog = dog_image->output_3()->clone();
  cv::resize(dog, dog, cv::Size(640, 480));
  cv::imshow("dog", dog);
  cv::waitKey(0);

  return 0;
}
