#pragma once
#include <opencv2/opencv.hpp>

uchar quantize_pixel(uchar pixel, int n);
void floyd_steinberg_dithering_grayscale(cv::Mat & img, int n);
void floyd_steinberg_dithering_color(cv::Mat & img, int n);