#include "CameraImageItem.h"
#include <QPainter>
#include <QDebug>
CameraImageItem::CameraImageItem(QGraphicsItem *parent)
	: QGraphicsItem(parent)
{
}
QRectF CameraImageItem::boundingRect()const{
	const int ADJUST = 2;
	return QRectF(-m_frame.width()/2-ADJUST,-m_frame.height()/2-ADJUST,
		m_frame.width()+ADJUST*2,m_frame.height()+ADJUST*2);
}
void CameraImageItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ){
	QRectF boundRect = boundingRect();
	m_imageMutex.lock();
	painter->drawImage(boundRect.topLeft(),m_frame);
	m_imageMutex.unlock();
}
void CameraImageItem::updateImage(QImage& frame){
	m_imageMutex.lock();
	m_frame = frame;
	m_imageMutex.unlock();
}
CameraImageItem::~CameraImageItem()
{

}
