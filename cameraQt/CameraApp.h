#ifndef CameraApp_H
#define CameraApp_H

#include <QMainWindow>
#include <QMutex>
#include <vector>
#include "ui_CameraApp.h"
#include "ImageProcess.h"
class QGraphicsView;
class CameraImageItem;

class CameraApp : public QMainWindow
{
	Q_OBJECT

public:
    CameraApp(QWidget *parent = 0);
    ~CameraApp();
	void updateImage(QImage& frame);
    ImageProcess* getImageProcessFunction()const;

private:
    Ui::CameraAppClass ui;
    CameraImageItem* m_cameraImageItem;
	QGraphicsView* m_view;
    ImageProcess* m_processFunction;
    mutable QMutex m_pfMutex;
    std::vector<ImageProcess*> m_functionPool;

    void initFunctionPool();
 private slots:
    void selectFuntion(int index);
};

#endif // CameraApp_H
