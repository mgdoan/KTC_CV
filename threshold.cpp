#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <fstream> 

using namespace cv;
using namespace std;

void cout_grid_format(int num, ofstream &of){
    if(0 <= num && num <= 9){
        of << num << "   ";
    }
    else if(10 <= num && num <= 99){
        of << num << "  ";
    }
    else{
        of << num << " "; 
    }
}


int main( int argc, char** argv )
{
    Mat imgOriginal = imread("/Users/meredithdoan/Desktop/opencv-setup/corrugated.jpeg");
     
    if (imgOriginal.empty()) {
        cerr << "Error: could not read image" << endl;
        return -1;
    }
    
    namedWindow("Control", WINDOW_AUTOSIZE);

    int iLowH = 1;
    int iHighH = 30;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

   createTrackbar("LowH", "Control", &iLowH, 179);
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 50);
    createTrackbar("HighS", "Control", &iHighS, 50);

    createTrackbar("LowV", "Control", &iLowV, 50);
    createTrackbar("HighV", "Control", &iHighV, 50);

    createTrackbar("LowH", "Control", nullptr, 179);
    createTrackbar("HighH", "Control", nullptr, 179);

    createTrackbar("LowS", "Control", nullptr, 255);
    createTrackbar("HighS", "Control", nullptr, 255);

    createTrackbar("LowV", "Control", nullptr, 255);
    createTrackbar("HighV", "Control", nullptr, 255);

    ofstream R;
    R.open("R.txt");
    ofstream G;
    G.open("G.txt");
    ofstream B;
    B.open("B.txt");

    if (!R.is_open() || !G.is_open() || !B.is_open()) {
        cout << "Error: Failed to open the file" << endl;
        return -1;
    }


    for (int y = 0; y < imgOriginal.rows; y++) {
        for (int x = 0; x < imgOriginal.cols; x++) {
            Vec3b rgb = imgOriginal.at<Vec3b>(y, x);
            int r = rgb[0];  // Hue
            int g = rgb[1];  // Saturation
            int b = rgb[2];  // Value
            cout_grid_format(r, R);
            cout_grid_format(g, G);
            cout_grid_format(b, B);
        }
        R << endl;
        G << endl;
        B << endl;
    }

    R.close();
    G.close();
    B.close();



    Mat imgHSV;
    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);

    ofstream H;
    H.open("H.txt");
    ofstream S;
    S.open("S.txt");
    ofstream V;
    V.open("V.txt");

    if (!H.is_open() || !S.is_open() || !V.is_open()) {
        cout << "Error: Failed to open the file" << endl;
        return -1;
    }


    for (int y = 0; y < imgOriginal.rows; y++) {
        for (int x = 0; x < imgOriginal.cols; x++) {
            Vec3b hsv = imgHSV.at<Vec3b>(y, x);
            int h = hsv[0];  // Hue
            int s = hsv[1];  // Saturation
            int v = hsv[2];  // Value
            cout_grid_format(h, H);
            cout_grid_format(s, S);
            cout_grid_format(v, V);
        }
        H << endl;
        S << endl;
        V << endl;
    }

    H.close();
    S.close();
    V.close();

    Mat imgThresholded;
    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);

    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    imshow("Thresholded Image", imgThresholded);
    imshow("Original", imgOriginal);

    waitKey(0);

    return 0;
}

