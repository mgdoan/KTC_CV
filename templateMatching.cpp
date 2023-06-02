#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream> 
#include <cassert>

using namespace cv;
using namespace std;
int main() {
    cv::Mat img_rgb = cv::imread("/Users/meredithdoan/Desktop/opencv-setup/patternTest2.PNG"); // Read RGB image
    assert(!img_rgb.empty());

    cv::Mat img_gray;
    cv::cvtColor(img_rgb, img_gray, cv::COLOR_BGR2GRAY); // Convert RGB image to grayscale

    cv::Mat template_img = cv::imread("/Users/meredithdoan/Desktop/opencv-setup/test_crop.PNG", cv::IMREAD_GRAYSCALE); // Read template image
    assert(!template_img.empty());

    int w = template_img.cols; // Width of the template image
    int h = template_img.rows; // Height of the template image

    cv::Mat result;
    cv::matchTemplate(img_gray, template_img, result, cv::TM_CCOEFF_NORMED); // Perform template matching
    double threshold = 0.9;
    //correct threshold for pts number: 0.9999

    cv::Mat locations;
    cv::threshold(result, locations, threshold, 1.0, cv::THRESH_BINARY); // Find locations where the result is above the threshold

    std::vector<cv::Point> pts;
    cv::findNonZero(locations, pts); // Find non-zero points in the locations

    for (const auto& pt : pts) {
        cv::rectangle(img_rgb, pt, cv::Point(pt.x + w, pt.y + h), cv::Scalar(0, 0, 255), 2); // Draw rectangles around the matched templates
    }

    cout << "pattern identified " << pts.size() << " time(s)" << endl;

    cv::imwrite("res.png", img_rgb); // Save the result image

    return 0;
}