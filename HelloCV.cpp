#include <stdio.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat img, img_g, img_d;
int threshold_value = 50;
const std::string w_name = "Open CV Lab";

void Threshold_Change( int, void* ){
    cv::threshold(img_g, img_d, threshold_value, 255, cv::THRESH_OTSU);
    cv::imshow(w_name, img_d);
    // printf("%i \n", threshold_value);
}

int main(int argc, char *argv[]){ 
    std::string imageName = "";

    if (argc > 1)
    {
        imageName = argv[1];
    }

    img = cv::imread( imageName , cv::IMREAD_UNCHANGED );
    
    if (img.empty()){
        printf("Cannot read the image");
        return -1;
    }

    cv::namedWindow(w_name, cv::WINDOW_AUTOSIZE );    

    cv::cvtColor(img, img_g, cv::COLOR_BGR2GRAY);

    cv::threshold(img_g, img_d, 0, 255, cv::THRESH_OTSU);

    cv::imshow(w_name, img_d);

    cv::createTrackbar("Threshold Value", w_name, &threshold_value, 255, Threshold_Change);

    Threshold_Change( 0, 0 );

    cv::waitKey();
    return 0;
}