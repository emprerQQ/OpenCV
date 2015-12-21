#ifndef CAMERAIMAGEITEM_H
#define CAMERAIMAGEITEM_H

#include <QGraphicsItem>
#include <QMutex>
class CameraImageItem : public QGraphicsItem
{

public:
	CameraImageItem(QGraphicsItem *parent = NULL);
	QRectF boundingRect()const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
	void updateImage(QImage& frame);
	~CameraImageItem();

private:
	QImage m_frame;
	QMutex m_imageMutex;
};

#endif // CAMERAIMAGEITEM_H
