#include "CameraThread.h"
#include "CameraApp.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <asm/types.h>    
#include <errno.h>
using namespace std;
const int DELAY = 30;
CameraThread::CameraThread(QObject *parent,CameraApp* win)
    : QThread(parent),m_mainWindow(win),m_camera("/dev/video0")
{
}
CameraThread::~CameraThread()
{

}
void CameraThread::run(){
    if(!m_camera.startCapture()){
		cerr << "can't start capture" << endl;
	}
	while(true){
		msleep(DELAY);
		uchar* imgData = m_camera.capture_image();
		if(imgData == NULL)continue;
		Mat image(IMAGE_HEIGHT,IMAGE_WIDTH,CV_8UC3,imgData);
        Mat proImage;
        ImageProcess* imgProcess = m_mainWindow->getImageProcessFunction();
        if(imgProcess){
            proImage = imgProcess->process(image);
        }else{
            proImage = image;
        }
        QImage frame = Mat2QImage(proImage);
        m_mainWindow->updateImage(frame);
        delete [] imgData; //image 并没有管理imgData,需要自己手动删除
	}

}

QImage Mat2QImage(cv::Mat const& src)
{
    cv::Mat temp = src; // make the same cv::Mat
  //  cvtColor(src, temp,CV_RGB2GRAY); // cvtColor Makes a copy, that what i need
	
	//fixme 应该可以防止一次拷贝，Mat应该有给出data所属权的函数
	switch(temp.type()){
		case CV_8UC1:
		{
			QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Indexed8);
			QVector<QRgb> my_table;
			for(int i = 0; i < 256; i++) my_table.push_back(qRgb(i,i,i));
			dest.setColorTable(my_table);
			dest.bits(); // enforce deep copy, see documentation 
			return dest;
		}
			break;
		case CV_8UC3:
		{
			QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
			dest.bits();
			return dest;
		}
			break;
		default:
			break;
	};
	return QImage();
	// of QImage::QImage ( const uchar * data, int width, int height, Format format )
}
