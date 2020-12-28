/**
 * @file ImageAlignment.hpp
 * @author Samuel Leong (samleocw@gmail.com)
 * @brief Image Alignment class which implements Baker-Matthews inverse
 * compositional image alignment
 *
 * @version 0.1
 * @date 2020-12-25
 *
 * @copyright Copyright (c) 2020
 */

#ifndef __IMAGE_ALIGNMENT_H__
#define __IMAGE_ALIGNMENT_H__

#include <Eigen/Dense>
#include <iostream>
#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

/// @brief BBOX Type: Simple TLBR array
typedef float bbox_t[4];

/**
 * @brief Image Alignment Class
 *
 * Uses Baker-Matthews Inverse Compositional tracking algorithm
 * along with robust M-estimator to handle illumination differences
 *
 * Handles the initialisation of a tracker with appropriate (sub-pixel) BBOX
 * Parent program calls `track()` to track the template image in the next frame
 * Updating the BBOX accordingly
 */
class ImageAlignment {
  private:
    /// @brief BBOX of template image (top, left, bottom, right)
    bbox_t mBbox;

    /// @brief Template Image (previous frame)
    cv::Mat mTemplateImage;

    /// @brief Current Image (current frame)
    cv::Mat mCurrentImage;

    void printCVMat(cv::Mat &aMat, std::string aName = "CV Matrix");

  public:
    // Constructor
    ImageAlignment();
    ImageAlignment(const cv::Mat &aImage);
    ImageAlignment(const bbox_t &aBbox);
    ImageAlignment(const cv::Mat &aImage, const bbox_t &aBbox);

    // Init
    void init(const cv::Mat &aImage);
    void init(const bbox_t &aBbox);
    void init(const cv::Mat &aImage, const bbox_t &aBbox);

    // BBOX Interface
    bbox_t &getBBOX();
    void setBBOX(const bbox_t &aBbox);
    void setBBOX(const float aTop, const float aLeft, const float aBottom,
                 const float aRight);

    // Get and Set Images
    cv::Mat &getTemplateImage();
    void setTemplateImage(const cv::Mat &aImg);

    cv::Mat &getCurrentImage();
    void setCurrentImage(const cv::Mat &aImg);

    // Display with (or without) BBOX
    void displayCurrentImage(const bool aWithBBOX = true,
                             const std::string &aTitle = "Current Image",
                             const cv::Scalar &aBBOXColour = cv::Scalar(0, 0,
                                                                        255),
                             const int aThickness = 3);

    // Get sub pixel value of image
    float getSubPixelValue(cv::Mat &aImg, float ax, float ay);

    // Track
    void track(const cv::Mat &aNewImage, const float aThreshold = 0.01875,
                const size_t aMaxIters = 100);
};

#endif