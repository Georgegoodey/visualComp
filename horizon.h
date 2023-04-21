#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>

std::vector<double> fitPoly(std::vector<cv::Point> points, int n);

cv::Point pointAtX(std::vector<double> coeff, double x);