#include "ImageAlignment.hpp"

/**
 * @brief Constructor for ImageAlignment class (empty)
 */
ImageAlignment::ImageAlignment() {}

/**
 * @brief Constructor for ImageAlignment class
 *
 * @param[in] aImage Initial current image
 */
ImageAlignment(cv::Mat &aImage) {
    init(aImage);
}

/**
 * @brief Constructor for ImageAlignment class
 *
 * @param[in] aBbox Initial BBOX
 */
ImageAlignment(bbox_t &aBbox) {
    init(aBbox);
}

/**
 * @brief Constructor for ImageAlignment class
 *
 * @param[in] aImage Initial current image
 * @param[in] aBbox Initial BBOX
 */
ImageAlignment(cv::Mat &aImage, bbox_t &aBbox) {
    init(aImage, aBbox);
}

/**
 * @brief Initialiser
 * @param[in] aImage Initial current image
 */
void ImageAlignment::init(cv::Mat &aImage) {
    setCurrentImage(aImage);
}

/**
 * @brief Initialiser
 * @param[in] aBbox Initial BBOX
 */
void ImageAlignment::init(bbox_t &aBbox) {
    setBBOX(aBbox);
}

/**
 * @brief Initialiser
 *
 * @param[in] aImage Initial current image
 * @param[in] aBbox Initial BBOX
 */
void ImageAlignment::init(cv::Mat &aImage, bbox_t &aBbox) {
    setCurrentImage(aImage);
    setBBOX(aBbox);
}

// https://docs.opencv.org/master/da/d54/group__imgproc__transform.html#ga0203d9ee5fcd28d40dbc4a1ea4451983

/**
 * @brief Get BBOX (top, left, bottom, right)
 *
 * @return bbox_t current BBOX
 */
bbox_t &ImageAlignment::getBBOX() {
    return mBbox;
}

/**
 * @brief Set BBOX (top, left, bottom, right)
 *
 * @param[in] aBbox BBOX
 */
void ImageAlignment::setBBOX(bbox_t &aBbox) {
    for (int i = 0; i < 4; i++)
        mBbox[i] = aBbox[i];
}

/**
 * @brief Set BBOX (top, left, bottom, right)
 *
 * @param[in] aTop Top of BBOX
 * @param[in] aLeft Left of BBOX
 * @param[in] aBottom Bottom of BBOX
 * @param[in] aRight Right of BBOX
 */
void ImageAlignment::setBBOX(float aTop, float aLeft, float aBottom,
                             float aRight) {
    mBbox[0] = aTop;
    mBbox[1] = aLeft;
    mBbox[2] = aBottom;
    mBbox[3] = aRight;
}

/**
 * @brief Get template image (ie prev frame)
 */
cv::Mat &ImageAlignment::getTemplateImage() {
    return mTemplateImage;
}

/**
 * @brief Set template image
 * @param[in] aImg Template image
 */
void ImageAlignment::setTemplateImage(cv::Mat &aImg) {
    mTemplateImage = aImg;
}

/**
 * @brief Get current image
 */
cv::Mat &ImageAlignment::getCurrentImage() {
    return mTemplateImage;
}

/**
 * @brief Set current image
 * @param[in] aImg Current image
 */
void ImageAlignment::setCurrentImage(cv::Mat &aImg) {
    mTemplateImage = aImg;
}

/**
 * @brief Using the iteratively saved BBOX, get template from "current" frame
 * (which is the previous frame) and perform Baker-Matthews IC image alignment:
 *
 * Proceed to update new bbox (detection) accordingly
 *
 * @param[in] aNewImage New image to track in
 * @param[in] aThreshold Threshold to compare against
 * @param[in] aMaxIters
 */
void ImageAlignment::track(cv::Mat &aNewImage, float aThreshold = 0.01875,
                           unsigned int aMaxIters = 100) {
    // Set new images
    //  - "Current" image becomes template
    //  - New image becomes current image
    cv::Mat &templateImage = getCurrentImage();
    setTemplateImage(templateImage);
    setTemplateImage(aNewImage);

    // Get BBOX
    // bbox_t &bbox = getBBOX();
    bbox_t &bbox = mBbox;
    cv::Size2d bboxSize(bbox[2] - bbox[0], bbox[3] - bbox[1]);
    cv::Point2f bboxCenter((bbox[2] + bbox[0]) / 2, (bbox[3] + bbox[1]) / 2);

    // Subpixel crop
    // Get actual template
    cv::Mat template(bboxSize, CV_64F);
    cv::getRectSubPix(templateImage, bboxSize, bboxCenter, template, CV_64F);

    // Get template image gradients
    cv::Mat templateGradX, templateGradY;
    cv::Sobel(template, templateGradX, CV_64F, 1, 0);
    cv::Sobel(template, templateGradY, CV_64F, 0, 1);

    std::cout << "T_X\n" << templateGradX << std::endl;
    std::cout << "T_Y\n" << templateGradY << std::endl;

    // Precompute Jacobian and Hessian

    Eigen::Matrix3d warpMat = Eigen::Matrix3d::Identity();
}
