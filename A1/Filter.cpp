#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <cmath>
#include <iostream>
#include <ostream>


// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // 1. Copy the original image for reference.
    GrayscaleImage originalImage = image;

    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    int halfKernel = kernelSize / 2;

    for(int i=0; i<image.get_height(); i++){
        for(int j=0; j<image.get_width(); j++){
            double sum = 0.0;
            for (int ki = -halfKernel; ki <= halfKernel; ki++) {
                for (int kj = -halfKernel; kj <= halfKernel; kj++) {
                    if(i + ki >=0 && i + ki<image.get_height() &&
                        j + kj >= 0 && j + kj<image.get_width()){
                        sum += originalImage.get_pixel(i +ki, j+kj);
                    }
                }
            }

            double mean = sum / (kernelSize*kernelSize);

            // 3. Update each pixel with the computed mean.
            image.set_pixel(i, j, static_cast<int>(mean));
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // 1. Create a Gaussian kernel based on the given sigma value.
    int halfKernel = kernelSize / 2;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize, 0.0));
    double sum = 0.0;
    for (int i = -halfKernel; i <= halfKernel; i++) {
        for (int j = -halfKernel; j <= halfKernel; j++) {
            kernel[i + halfKernel][j + halfKernel] = 1/ (2 * M_PI * sigma * sigma) * exp(-(i * i + j * j) / (2 * sigma * sigma)) ;
            sum += kernel[i + halfKernel][j + halfKernel];
        }
    }

    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }

    // 3. For each pixel, compute the weighted sum using the kernel.
    GrayscaleImage originalImage = image;
    for(int i=0; i<image.get_height(); i++){
        for(int j=0; j<image.get_width(); j++){
            double weightedSum = 0.0;
            for (int ki = -halfKernel; ki <= halfKernel; ki++) {
                for (int kj = -halfKernel; kj <= halfKernel; kj++) {
                   if(i+ki >=0 && i+ki<image.get_height() &&
                        j+kj >= 0 && j+kj<image.get_width()){
                        weightedSum +=
                            originalImage.get_pixel(i+ki, j+kj)
                                * kernel[ki + halfKernel][kj + halfKernel];
                    }
                }
            }
            // 4. Update the pixel values with the smoothed results.
            image.set_pixel(i, j, static_cast<int>(weightedSum));
        }
    }

}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    GrayscaleImage originalImage = image;
    apply_gaussian_smoothing(image, kernelSize, 1.0);
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    for(int i=0; i<image.get_height(); i++){
        for(int j=0; j<image.get_width(); j++){
            int original = originalImage.get_pixel(i, j);
            int blurred = image.get_pixel(i, j);
            int unsharp = static_cast<int>(original + amount * (original-blurred));

            // 3. Clip values to ensure they are within a valid range [0-255].
            image.set_pixel(i, j, unsharp < 0 ? 0 : (unsharp > 255 ? 255 : unsharp));
        }
    }
}
