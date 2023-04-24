#include <stdio.h> 
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "horizon.h"

using namespace cv;
using namespace std;

// Mat declarations
Mat img, img_g, img_c, img_h, img_h_py, img_h_py_hoz, img_poly;

// Name for OTSU Window
const string w1_name = "Canny transformation";
// Name for Binary Window
const string w2_name = "Probabilistic Hough Lines";
// Name for Binary Window
const string w3_name = "Long Lines";
// Name for Binary Window
const string w4_name = "Horizontal Lines";
// Name for Binary Window
const string w5_name = "Polynomial Line";

int main( int argc, char *argv[]){
    // Default name to nothing so file read will not fail if not overwritten
    string imageName("");

    // If enough args then image was provided
    if (argc > 1){
        // Set imageName to first arg
        imageName = argv[1];
    }else{
        // If not enough args then no image link is given
        // Print error message
        printf("No image given");
        // Return fail code
        return -1;
    }

    // Try to load argument image into img mat
    img = imread(imageName, IMREAD_UNCHANGED);

    // If img is null then .empty() will return true and image is invalid
    if (img.empty()){
        // Print error message
        printf("Cannot read the image");
        // Return fail
        return -1;
    }

    // Convert the original Image to Gray
    cvtColor(img, img_g, COLOR_BGR2GRAY);

    threshold(img_g, img_g, 122, 255, THRESH_BINARY);
    Canny(img, img_c, 255/3, 255, 3);

    // Show output of canny in window
    imshow(w1_name, img_c);

    img_h = img.clone();
    img_h_py = img.clone();
    img_h_py_hoz = img.clone();
    img_poly = img.clone();

    vector<Vec4i> lines_h; // will hold the results of the detection
    HoughLinesP(img_c, lines_h, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection

    for( size_t i = 0; i < lines_h.size(); i++ )
    {
        Vec4i l = lines_h[i];
        line( img_h, Point(l[0], l[1]), Point(l[2], l[3]), (0,0,255));
    }

    // Show output of thresholding in window
    imshow(w2_name, img_h);

    vector<Vec4i> lines_h_py;
    for( size_t i = 0; i < lines_h.size(); i++ )
    {
        Vec4i l = lines_h[i];
        if(pow(l[0]-l[2],2)+pow(l[1]-l[3],2)>pow(120,2)){
            lines_h_py.push_back(l);
        }
    }

    for( size_t i = 0; i < lines_h_py.size(); i++ )
    {
        Vec4i l = lines_h_py[i];
        line( img_h_py, Point(l[0], l[1]), Point(l[2], l[3]), (0,0,255));
    }

    // Show output of thresholding in window
    imshow(w3_name, img_h_py);

    vector<Vec4i> lines_h_py_hoz;
    for( size_t i = 0; i < lines_h_py.size(); i++ )
    {
        Vec4i l = lines_h_py[i];
        double calc = abs(atan2(l[0]-l[2],l[1]-l[3]));
        if(!(calc<CV_PI/4)&&!(calc>CV_PI*3/4)){
            lines_h_py_hoz.push_back(l);
        }
    }

    for( size_t i = 0; i < lines_h_py_hoz.size(); i++ )
    {
        Vec4i l = lines_h_py_hoz[i];
        line( img_h_py_hoz, Point(l[0], l[1]), Point(l[2], l[3]), (0,0,255));
    }

    // Show output of thresholding in window
    imshow(w4_name, img_h_py_hoz);

    vector<Point> points_h_py_hoz;
    for( size_t i = 0; i < lines_h_py_hoz.size(); i++ )
    {
        Vec4i l = lines_h_py_hoz[i];
        Point p1 = Point(l[0],l[1]);
        Point p2 = Point(l[2],l[3]);
        points_h_py_hoz.push_back(p1);
        points_h_py_hoz.push_back(p2);
    }

    vector<double> coeffs;
    coeffs = fitPoly(points_h_py_hoz, 2);
    for( size_t i = 0; i < img_poly.cols; i++ )
    {
        Point point = pointAtX(coeffs, i);
        circle(img_poly, point, 0, (0, 0, 255), -1);
    }

    // Show output of thresholding in window
    imshow(w5_name, img_poly);

    // Wait for user input so doesn't end straight away
    waitKey(0);

    // If all code works return success
    return 0;
}

