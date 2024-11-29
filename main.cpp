#include "floyd.h"
#include "stucki.h"
#include <opencv2/opencv.hpp>
#include <iostream>



int main() {
    // original file name
    std::string inputFileName = "cat";

    // Load the grayscale image
    cv::Mat img = cv::imread("/Users/hisoka/Downloads/graphics2/" + inputFileName + ".png", cv::IMREAD_GRAYSCALE);
    // Load the colored image
    //cv::Mat img = cv::imread("/Users/hisoka/Downloads/graphics2/" + inputFileName + ".png");

    if (img.empty()) {
        std::cerr << "Error: Could not open or find the image!" << std::endl;
        return -1;
    }

    int n = 1; // Number of bits per pixel

    floyd_steinberg_dithering_grayscale(img, n);
    //floyd_steinberg_dithering_color(img, n);
    //stucki_dithering_grayscale(img, n);
    //stucki_dithering_color(img, n);

    std::string outputFileName = inputFileName + "_floyd_gray" + std::to_string(n) + ".png";
    //std::string outputFileName = inputFileName + "_floyd_color" + std::to_string(n) + ".png";
    //std::string outputFileName = inputFileName + "_stucki_color" + std::to_string(n) + ".png";
    //std::string outputFileName = inputFileName + "_stucki_gray" + std::to_string(n) + ".png";

    // Save the output image
    cv::imwrite(outputFileName, img);

    return 0;
}
