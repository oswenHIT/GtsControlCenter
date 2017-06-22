#include "iconeditor.h"

IconEditor::IconEditor(QWidget *parent) 
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	
	curColor = Qt::black;
	zoom = 8;

	image = QImage(16, 16, QImage::Format_ARGB32);
	QRgb white = qRgba(0, 0, 0, 0);
	image.fill(white);
}

QSize IconEditor::sizeHint() const
{
	QSize size = zoom * image.size();

	if (zoom > 3)
		size += QSize(1, 1);

	return size;
}

void IconEditor::setPenColor(const QColor &newColor)
{
	curColor = newColor;
}

void IconEditor::setIconImage(const QImage &newImage)
{
	if (image != newImage)
	{
		image = newImage.convertToFormat(QImage::Format_ARGB32);
		update();
		updateGeometry();
	}
}

void IconEditor::setZoomFactor(int newZoom)
{
	if (newZoom < 1)
	{
		newZoom = 1;
	}

	if (newZoom != zoom)
	{
		zoom = newZoom;
		update();
		updateGeometry();
	}
}

void IconEditor::paintEvent(QPaintEvent *qevent)
{
	QPainter painter(this);

	if (zoom > 3)
	{
		painter.setPen(palette().foreground().color());

		for (int i = 0; i < image.width(); i++)
		{
			painter.drawLine(zoom * i, 0, zoom * i, image.height());
		}

		for (int j = 0; j < image.height(); j++)
		{
			painter.drawLine(0, zoom * j, zoom * image.width(), zoom * j);
		}
	}

	for (int i = 0; i < image.width(); i++)
	{
		for (int j = 0; j < image.height(); j++)
		{
			QRect rect = pixelRect(i, j);
			
			if (!qevent->region().intersected(rect).isEmpty())
			{
				QColor color = QColor::fromRgba(image.pixel(i, j));

				if (color.alpha() < 255)
				{
					painter.fillRect(rect, Qt::white);
				}

				painter.fillRect(rect, color);
			}
		}
	}

}

void IconEditor::mousePressEvent(QMouseEvent *mevent)
{
	if (mevent->button() == Qt::LeftButton)
	{
		setImagePixel(mevent->pos(), true);
	}
	else if (mevent->button() == Qt::RightButton)
	{
		setImagePixel(mevent->pos(), false);
	}
}

void IconEditor::mouseMoveEvent(QMouseEvent *mevent)
{
	if (mevent->button() & Qt::LeftButton)
	{
		setImagePixel(mevent->pos(), true);
	}
	else if (mevent->button() & Qt::RightButton)
	{
		setImagePixel(mevent->pos(), false);
	}
}

void IconEditor::setImagePixel(const QPoint &pos, bool opaque)
{
	int i = pos.x() / zoom;
	int j = pos.y() / zoom;

	if (image.rect().contains(i, j))
	{
		if (opaque)
		{
			image.setPixel(i, j, penColor().rgba());
		}
		else
		{
			image.setPixel(i, j, qRgba(0, 0, 0, 0));
		}

		update(pixelRect(i, j));
	}
}

QRect IconEditor::pixelRect(int i, int j) const
{
	if (zoom >= 3)
	{
		return QRect(zoom*i + 1, zoom*j + 1, zoom - 1, zoom - 1);
	}
	else
	{
		return QRect(zoom * i, zoom * j, zoom, zoom);
	}
}

