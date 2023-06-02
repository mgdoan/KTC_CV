#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream> 

using namespace cv;
using namespace std;


Mat src_gray;
int thresh = 1;
RNG rng(12345);
void thresh_callback(int, void*, int rows, int cols);
int main( int argc, char** argv )
{
    Mat src = imread("/Users/meredithdoan/Desktop/opencv-setup/patternTest2.PNG");

    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
    const char* source_window = "Source";
    namedWindow( source_window );
    imshow( source_window, src );
    const int max_thresh = 255;


    createTrackbar("Canny thresh:", source_window, nullptr, max_thresh,[](int value, void*) {
        thresh = value;
    },nullptr);

    int rows = src.rows;
    int cols = src.cols;

    thresh_callback(0, 0, rows, cols);
    waitKey();
    return 0;
}
void thresh_callback(int, void*, int rows, int cols)
{
    Mat canny_output;
    Canny( src_gray, canny_output, thresh, thresh*200 );
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

   /* double minAreaThreshold = 20;

    std::vector<std::vector<cv::Point>> filteredContours;
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > minAreaThreshold) {  // Set an appropriate minimum area threshold
            filteredContours.push_back(contour);
        }
    }
    */
   
    double minLengthThreshold = 4000;
    //CORRECT THRESHOLD FOR patternTest2: 4,000
    //CORRECT THRESHOLD FOR patternTest1: 

    std::vector<std::vector<cv::Point>> filteredContours;
    for (const auto& contour : contours) {
        double length = cv::arcLength(contour, true);
        if (length > minLengthThreshold) {  // Set an appropriate minimum length threshold
            filteredContours.push_back(contour);
        }
    }


    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< filteredContours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( drawing, filteredContours, (int)i, color, 2, LINE_8, hierarchy, 0 );
    }
    
    int verticalContourCount = 0;
    //int verticalLineX = cols / 2 +50; // X-coordinate of the vertical line
    int verticalLineY1 = 0; // Y-coordinate of the top of the vertical line
    int verticalLineY2 = rows; // Y-coordinate of the bottom of the vertical line
    int x_count = 0;

    for(int verticalLineX = 0; verticalLineX < cols; verticalLineX++){
        for (const auto& contour : filteredContours)
            {
            cv::Rect boundingRect = cv::boundingRect(contour);
            int contourCenterX = boundingRect.x + boundingRect.width / 2;
            int contourTopY = boundingRect.y;
            int contourBottomY = boundingRect.y + boundingRect.height;

            if (contourCenterX >= verticalLineX && contourTopY <= verticalLineY2 && contourBottomY >= verticalLineY1)
            {
            verticalContourCount++;
            }
        }
        x_count++;
        //line(drawing, Point(verticalLineX, 0), Point(verticalLineX, rows), Scalar(0, 255, 0), 2);
    }

    line(drawing, Point(cols / 2 +50, 0), Point(cols / 2 +50, rows), Scalar(0, 255, 0), 2);
    //cout << "Number of vertically intersecting contours: " << verticalContourCount << endl;
    cout << "vertical contour count: " << verticalContourCount << endl;
    cout << "x count: " << x_count << endl;
    cout << "Number of sheets detected: " << verticalContourCount / x_count  << endl;
    cout << "Correct number of sheets: " << endl;
    cout << "Number of contours: " << filteredContours.size() << endl;

    imshow( "Contours", drawing );
}


