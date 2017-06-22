#ifndef DIYSCROLLAREA_H
#define DIYSCROLLAREA_H

#include <qwidget.h>
#include <qimage.h>
#include <qevent.h>
#include <qpoint.h>
#include <qlabel.h>
#include <qpalette.h>
#include <qscrollarea.h>
#include <qscrollbar.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qmimedata.h>
#include <qvariant.h>
#include <qmessagebox.h>
#include <qdebug.h>

class DiyScrollArea : public QScrollArea
{
	Q_OBJECT

public:
	DiyScrollArea(QWidget *parent = 0);

	QPointF transformDist(QPoint &diff);
	void setImageSize(int width, int height);
	void setCurImageSize(int width, int height);
	void resetCoordinates();
	void changeImgSize(double scale);
	void loadImage(QString &imgName);

protected:
	void mousePressEvent(QMouseEvent *mevent);
	void mouseReleaseEvent(QMouseEvent *mevent);
	void mouseMoveEvent(QMouseEvent *mevent);
	void dragEnterEvent(QDragEnterEvent *devent);
	void dropEvent(QDropEvent *devent);
	void keyPressEvent(QKeyEvent *kevent);
	void wheelEvent(QWheelEvent *wevent);

private:
	QScrollBar *vbar;
	QScrollBar *hbar;
	QPoint lastPoint;
	QLabel *imgLabel;
	QImage image;

	int flag;
	int scaleEnable;
	int imgWidth;
	int imgHeight;
	int curImgWidth;
	int curImgHeight;
	int vValue;
	int hValue;
	double scale;
};


#endif