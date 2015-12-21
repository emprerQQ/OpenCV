#include "CameraApp.h"
#include "CameraThread.h"
#include <QApplication>
#include <QTimer>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    CameraApp w;
	w.show();
	CameraThread* cThread = new CameraThread(NULL,&w);
	cThread->start();
	return a.exec();
}

