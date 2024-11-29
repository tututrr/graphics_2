#include "stucki.h"
#include "floyd.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;


void stucki_dithering_grayscale(Mat& img, int n) {

    Mat out_img = Mat::zeros(img.size(), CV_8U);

    //Stucki dithering matrix
    int matrix[3][5] = {
        {0, 0, 0, 8, 4},
        {2, 4, 8, 4, 2},
        {1, 2, 4, 2, 1}
    };

    // iterate over each pixel in the image
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            // Get the original pixel value
            uchar pixel = img.at<uchar>(y, x);
            // Quantize the pixel, reducing the number of bits
            uchar quantized_pixel = quantize_pixel(pixel, n);
            // Calculate the error
            int error = pixel - quantized_pixel;
            // Distribute the error to neighboring pixels
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -2; dx <= 2; dx++) {
                    if (x + dx >= 0 && x + dx < img.cols && y + dy >= 0 && y + dy < img.rows) {
                        if (matrix[dy + 1][dx + 2] != 0) {
                            uchar& neighbor_pixel = img.at<uchar>(y + dy, x + dx);
                            neighbor_pixel = std::min(255, std::max(0, neighbor_pixel + error * matrix[dy + 1][dx + 2] / 42));
                        }
                    }
                }
            }

            // Set the quantized pixel value in the output image
            out_img.at<uchar>(y, x) = quantized_pixel;
        }
    }

    img = out_img;
}


void stucki_dithering_color(Mat& img, int n) {

    Mat out_img = Mat::zeros(img.size(), img.type());

    // define the Stucki dithering matrix
    int matrix[3][5] = {
        {0, 0, 0, 8, 4},
        {2, 4, 8, 4, 2},
        {1, 2, 4, 2, 1}
    };

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
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -2; dx <= 2; dx++) {
                    if (x + dx >= 0 && x + dx < img.cols && y + dy >= 0 && y + dy < img.rows) {
                        if (matrix[dy + 1][dx + 2] != 0) {
                            Vec3b& neighbor_pixel = img.at<Vec3b>(y + dy, x + dx);
                            for (int i = 0; i < 3; i++) {
                                neighbor_pixel[i] = std::min(255, std::max(0, neighbor_pixel[i] + error[i] * matrix[dy + 1][dx + 2] / 42));
                            }
                        }
                    }
                }
            }

            // Set the quantized pixel value in the output image
            out_img.at<Vec3b>(y, x) = quantized_pixel;
        }
    }

    img = out_img;
}