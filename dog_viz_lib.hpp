#ifndef DOG_VIZ_LIB_HPP_
#define DOG_VIZ_LIB_HPP_

#include <opencv2/core/core.hpp>

class DogImage {
 public:
  using Ptr = std::unique_ptr<DogImage>;
  static Ptr create(const cv::Mat& rgb);

  const cv::Mat& rgb() const;
  const cv::Mat& r() const;
  const cv::Mat& g() const;
  const cv::Mat& b() const;
  cv::Mat* output_1();
  cv::Mat* output_3();

 private:
  DogImage(const cv::Mat& rgb,
           const cv::Mat& r,
           const cv::Mat& g,
           const cv::Mat& b,
           const cv::Mat& output_1,
           const cv::Mat& output_3);

  cv::Mat rgb_;
  cv::Mat r_;
  cv::Mat g_;
  cv::Mat b_;
  cv::Mat output_1_;
  cv::Mat output_3_;
};

void applyColorCorrection(DogImage& rgb);
void applyBrightnessCorrection(DogImage& rgb);
void applyAcuityCorrection(DogImage& rgb);

#endif // DOG_VIZ_LIB_HPP_
