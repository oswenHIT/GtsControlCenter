#ifndef IMGLABEL_H
#define IMGLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QString>

#include <tisudshl.h>

#include "Listener.h"
#include "assisstclass.h"

class ImgLabel : public QLabel
{
	Q_OBJECT

public:
	ImgLabel(QWidget *parent);
	~ImgLabel();

public:
	const wr::ROI & getROI() const;
	wr::ROI & getROI();
	void setListener(CListener* pLsn);

public:
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void setGrabMode(bool mode);
	void setGrabber(DShowLib::Grabber * grab);

private:
	wr::ROI  roi;
	CListener *pListener;
	DShowLib::Grabber * grabber;

private:
	bool isGrabMode;
	int originalX, originalY;
};

#endif // IMGLABEL_H
