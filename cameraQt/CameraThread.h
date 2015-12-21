#ifndef CameraThread_H
#define CameraThread_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <string>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "V4L2Driver.h"
class CameraApp;
using namespace cv;
QImage Mat2QImage(cv::Mat const& src);
class CameraThread : public QThread
{
	Q_OBJECT

public:
	CameraThread(QObject *parent,CameraApp*);
	~CameraThread();
	void run();
	
private:
	CameraApp* m_mainWindow;
	CameraV4L2 m_camera;
};
#endif // CameraThread_H
