#include <QGraphicsScene>
#include "CameraApp.h"
#include "CameraImageItem.h"
#include "V4L2Driver.h"
#include <iostream>
using namespace std;
CameraApp::CameraApp(QWidget *parent)
    : QMainWindow(parent),m_cameraImageItem(new CameraImageItem),m_processFunction(NULL)
{
	ui.setupUi(this);
	QGraphicsScene* scene = new QGraphicsScene;
	scene->setSceneRect(QRectF(-IMAGE_WIDTH/2,-IMAGE_HEIGHT/2,IMAGE_WIDTH,IMAGE_HEIGHT));
	scene->addItem(m_cameraImageItem);
	m_view = ui.graphicsView;
	m_view->setScene(scene);
	m_view->resize(IMAGE_WIDTH+20,IMAGE_HEIGHT+20);
    initFunctionPool();
    connect(ui.processComboBox,SIGNAL(activated(int)),this,SLOT(selectFuntion(int)));

}
CameraApp::~CameraApp()
{
    for(int i = m_functionPool.size() - 1;i>=0;i--){
        if(m_functionPool[i]){
            delete m_functionPool[i];
        }
    }
}
void CameraApp::updateImage(QImage& frame){
	m_cameraImageItem->updateImage(frame);
	m_view->viewport()->update();
}
ImageProcess* CameraApp::getImageProcessFunction()const{
    m_pfMutex.lock();
    ImageProcess* rst = m_processFunction;
    m_pfMutex.unlock();
    return rst;
}
void CameraApp::initFunctionPool()
{
    m_functionPool.push_back(NULL);
    m_functionPool.push_back(new ProcessGray);
    m_functionPool.push_back(new ProcessThreshold);
    m_functionPool.push_back(new ProcessCanny);
    m_functionPool.push_back(new RB_Reversal);
}

void CameraApp::selectFuntion(int index){
    m_pfMutex.lock();
    m_processFunction = m_functionPool[index];
    m_pfMutex.unlock();
}

