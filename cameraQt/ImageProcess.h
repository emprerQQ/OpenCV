#ifndef IMAGEPROCESS
#define IMAGEPROCESS
#include <opencv/cv.h>
using namespace cv;
class ImageProcess{
public:
    virtual Mat process(Mat&) = 0;
    virtual ~ImageProcess(){}
};
class ProcessGray : public ImageProcess{
public:
    virtual Mat process(Mat&);
};
class ProcessThreshold : public ImageProcess{
public:
    virtual Mat process(Mat&);
};
class ProcessCanny : public ImageProcess{
public:
    virtual Mat process(Mat&);
};
class RB_Reversal : public ImageProcess{
public:
    virtual Mat process(Mat&);
};
#endif // IMAGEPROCESS

