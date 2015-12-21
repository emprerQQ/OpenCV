#include <opencv/cv.h>
#include "ImageProcess.h"
using namespace cv;
Mat ProcessGray::process(Mat& src){
    Mat temp;

    cvtColor(src,temp,CV_RGB2GRAY);
    return temp;
}
Mat ProcessThreshold::process(Mat& src){
	Mat temp;
	cvtColor(src,temp,CV_RGB2GRAY);
	threshold(temp,temp,0,250, THRESH_OTSU);
    return temp;
}
Mat ProcessCanny::process(Mat& src){
	Mat temp;
	cvtColor(src,temp,CV_RGB2GRAY);
	Canny(temp,temp,125,250);
    return temp;
}
Mat RB_Reversal::process(Mat& src){
    Mat temp;
    cvtColor(src,temp,CV_BGR2RGB);
    return temp;
}
