#ifndef ICON_EDITOR_H
#define ICON_EDITOR_H

#include <QtWidgets/qwidget.h>
#include <qimage.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qevent.h>

class IconEditor : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
	Q_PROPERTY(QImage iconImage READ iconImage WRITE setIconImage)
	Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor)

public:
	IconEditor(QWidget *parent = 0);
	
	void setPenColor(const QColor &newColor);
	QColor penColor() const { return curColor; }

	void setZoomFactor(int newZoom);
	int zoomFactor() const { return zoom; }

	void setIconImage(const QImage &newImage);
	QImage iconImage() const { return image; }

	QSize sizeHint() const;

protected:
	void mousePressEvent(QMouseEvent *mevent);
	void mouseMoveEvent(QMouseEvent *mevent);
	void paintEvent(QPaintEvent *pevent);

private:
	void setImagePixel(const QPoint &pos, bool qpaque);
	QRect pixelRect(int i, int j) const;

	QColor curColor;
	QImage image;
	int zoom;
};


#endif