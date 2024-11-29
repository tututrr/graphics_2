#include "floyd.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

uchar quantize_pixel(uchar pixel, int n) {
    int color_count = 1 << n;
    int step = 255 / (color_count - 1);
    uchar max_clr = static_cast<uchar>(floor(255 / step) * step);
    uchar res;
    if (round(static_cast<float>(pixel) / (float)step) * step >= max_clr) res = 255; 
    else res = static_cast<uchar>(round(static_cast<float>(pixel) / (float)step) * step);
    return res;
}

void floyd_steinberg_dithering_grayscale(Mat& img, int n){
    Mat out_img = Mat::zeros(img.size(), CV_8U);

    // Iterate over each pixel in the image
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            // Get the original pixel value
            uchar pixel = img.at<uchar>(y, x);
            // Quantize the pixel, reducing the number of bitt
            uchar quantized_pixel = quantize_pixel(pixel, n);
            int error = pixel - quantized_pixel;
            // Distribute the error to neighboring pixels
            if (x < img.cols - 1) {
                // Add the error to the pixel to the right
                img.at<uchar>(y, x + 1) = std::min(255, std::max(0, img.at<uchar>(y, x + 1) + error * 7 / 16));
            }
            if (y < img.rows - 1) {
                // Add the error to the pixel below
                img.at<uchar>(y + 1, x) = std::min(255, std::max(0, img.at<uchar>(y + 1, x) + error * 5 / 16));
                if (x < img.cols - 1) {
                    // Add the error to the pixel below and to the right
                    img.at<uchar>(y + 1, x + 1) = std::min(255, std::max(0, img.at<uchar>(y + 1, x + 1) + error * 1 / 16));
                }
                if (x > 0) {
                    // Add the error to the pixel below and to the left
                    img.at<uchar>(y + 1, x - 1) = std::min(255, std::max(0, img.at<uchar>(y + 1, x - 1) + error * 3 / 16));
                }
            }

            // Set the quantized pixel value in the output image
            out_img.at<uchar>(y, x) = quantized_pixel;
        }
    }

    img = out_img;
}

void floyd_steinberg_dithering_color(Mat& img, int n) {

    Mat out_img = Mat::zeros(img.size(), img.type());

    // Iterate over each pixel in the image
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            // Get the original pixel value
            Vec3b pixel = img.at<Vec3b>(y, x);
            // Quantize the pixel, reducing the number of bits
            Vec3b quantized_pixel;
            for (int i = 0; i < 3; i++) {
                quantized_pixel[i] = quantize_pixel(pixel[i], n);
            }
            // Calculate the error
            Vec3b error;
            for (int i = 0; i < 3; i++) {
                error[i] = pixel[i] - quantized_pixel[i];
            }
            // Distribute the error to neighboring pixels
            if (x < img.cols - 1) {
                // Add the error to the pixel to the right
                Vec3b& right_pixel = img.at<Vec3b>(y, x + 1);
                for (int i = 0; i < 3; i++) {
                    right_pixel[i] = std::min(255, std::max(0, right_pixel[i] + error[i] * 7 / 16));
                }
            }
            if (y < img.rows - 1) {
                // Add the error to the pixel below
                Vec3b& below_pixel = img.at<Vec3b>(y + 1, x);
                for (int i = 0; i < 3; i++) {
                    below_pixel[i] = std::min(255, std::max(0, below_pixel[i] + error[i] * 5 / 16));
                }
                if (x < img.cols - 1) {
                    // Add the error to the pixel below and to the right
                    Vec3b& below_right_pixel = img.at<Vec3b>(y + 1, x + 1);
                    for (int i = 0; i < 3; i++) {
                        below_right_pixel[i] = std::min(255, std::max(0, below_right_pixel[i] + error[i] * 1 / 16));
                    }
                }
                if (x > 0) {
                    // Add the error to the pixel below and to the left
                    Vec3b& below_left_pixel = img.at<Vec3b>(y + 1, x - 1);
                    for (int i = 0; i < 3; i++) {
                        below_left_pixel[i] = std::min(255, std::max(0, below_left_pixel[i] + error[i] * 3 / 16));
                    }
                }
            }

            // Set the quantized pixel value in the output image
            out_img.at<Vec3b>(y, x) = quantized_pixel;
        }
    }

    img = out_img;
}
