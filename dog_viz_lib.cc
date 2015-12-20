#include <dog_viz_lib.hpp>

DogImage::DogImage(const cv::Mat& rgb,
                   const cv::Mat& r,
                   const cv::Mat& g,
                   const cv::Mat& b,
                   const cv::Mat& output_1,
                   const cv::Mat& output_3)
  : rgb_(rgb.clone()),
    r_(r.clone()),
    g_(g.clone()),
    b_(b.clone()),
    output_1_(output_1.clone()),
    output_3_(output_3.clone()) {
}

DogImage::Ptr DogImage::create(const cv::Mat& rgb) {

  if (rgb.channels() != 3)
    return nullptr;

  std::vector<cv::Mat> channels;
  cv::split(rgb, channels);

  cv::Mat b = channels.at(0);
  cv::Mat g = channels.at(1);
  cv::Mat r = channels.at(2);

  cv::Mat output_1 = cv::Mat(rgb.size(), CV_8UC1, cv::Scalar(0));
  cv::Mat output_3 = cv::Mat(rgb.size(), CV_8UC3, cv::Scalar::all(0));

  Ptr p(new DogImage(rgb, r, g, b, output_1, output_3));
  return p;
}

const cv::Mat& DogImage::rgb() const {
  return rgb_;
}

const cv::Mat& DogImage::r() const {
  return r_;
}

const cv::Mat& DogImage::g() const {
  return g_;
}

const cv::Mat& DogImage::b() const {
  return b_;
}

cv::Mat* DogImage::output_1() {
  return &output_1_;
}

cv::Mat* DogImage::output_3() {
  return &output_3_;
}

void applyColorCorrection(DogImage& img) {
  auto rgb = img.rgb();
  int channels = rgb.channels();
  int rows = rgb.rows;
  int cols = rgb.cols;

  if (rgb.isContinuous()) {
    cols *= rows;
    rows = 1;
  }

  int i = 0;
  int j = 0;
  uchar* p;
  for (i = 0; i < rows; ++i) {
    p = rgb.ptr<cv::Vec3b>(i);
  }

  const uint8_t* r = static_cast<const uint8_t*>(img.r().data);
  const uint8_t* g = static_cast<const uint8_t*>(img.g().data);
  const uint8_t* b = static_cast<const uint8_t*>(img.b().data);

  cv::Mat* output = img.output_3();

  for (int i = 0; i < img.rgb().cols * img.rgb().rows; ++i) {
    uint8_t* p = static_cast<uint8_t*>(img.output_3()->data);
    p[0] = b[i];
    p[2] = (g[i] + r[i]) / 2;
    p[1] = p[2];
  }
}

void applyBrightnessCorrection(DogImage& rgb) {
}

void applyAcuityCorrection(DogImage& rgb) {
}
