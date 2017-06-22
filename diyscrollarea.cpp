#include "diyscrollarea.h"

DiyScrollArea::DiyScrollArea(QWidget *parent)
	: QScrollArea(parent)
{
	flag = 0;
	scaleEnable = 0;
	imgWidth = 0;
	imgHeight = 0;
	curImgWidth = 0;
	curImgHeight = 0;
	vValue = 0;
	hValue = 0;
	imgLabel = new QLabel;

	vbar = this->verticalScrollBar();
	hbar = this->horizontalScrollBar();
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	lastPoint.setX(0);
	lastPoint.setY(0);

	setWidget(imgLabel);
	setAcceptDrops(true);
	setBackgroundRole(QPalette::Dark);
}

void DiyScrollArea::mousePressEvent(QMouseEvent *mevent)
{
	if (mevent->button() == Qt::RightButton)
	{
		lastPoint = mevent->pos();
		scaleEnable = 1;
	}
}

void DiyScrollArea::mouseReleaseEvent(QMouseEvent *mevent)
{
	if ((mevent->buttons() & Qt::RightButton) && scaleEnable)
	{
		vValue = vbar->value();
		hValue = hbar->value();
		scaleEnable = 0;
	}
}

void DiyScrollArea::mouseMoveEvent(QMouseEvent *mevent)
{
	int dx = 0;
	int dy = 0;
	QPoint tmpPoint;
	QPointF fp;

	if ((mevent->buttons() & Qt::RightButton) && scaleEnable)
	{
		++flag;
		if (flag == 4)
		{
			tmpPoint = mevent->pos() - lastPoint;
			fp = transformDist(tmpPoint);
			dx = static_cast<int>(fp.x());
			dy = static_cast<int>(fp.y());

			hbar->setValue(hbar->value() - 2 * dx);
			vbar->setValue(vbar->value() - 2 * dy);

			flag = 0;
			lastPoint = mevent->pos();
		}
	}
}

void DiyScrollArea::setImageSize(int width, int height)
{
	imgWidth = width;
	imgHeight = height;
}

void DiyScrollArea::setCurImageSize(int width, int height)
{
	curImgWidth = width;
	curImgHeight = height;
}

void DiyScrollArea::resetCoordinates()
{
	int vCor = static_cast<int>(vValue*curImgHeight / imgHeight);
	int hCor = static_cast<int>(hValue*curImgWidth / imgWidth);

	vbar->setValue(vCor);
	hbar->setValue(hCor);
}

QPointF DiyScrollArea::transformDist(QPoint &diff)
{
	int vMaximum = vbar->maximum();
	int hMaximum = hbar->maximum();
	QPointF result;

	result.setX(hMaximum*diff.x() / imgWidth);
	result.setY(vMaximum*diff.y() / imgHeight);

	return result;
}

void DiyScrollArea::wheelEvent(QWheelEvent *wevent)
{

	if (QApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		double degree = wevent->delta() / 8.0;
		double tmp = degree / 360.0;

		scale > 1.0 ?
			scale = 1.0 : scale < -0.96 ?
			scale = -0.96 : scale += tmp;

		changeImgSize(scale + 1.0);
	}

	return QScrollArea::wheelEvent(wevent);
}

void DiyScrollArea::changeImgSize(double scale)
{
	QImage tmpImg;

	int width = static_cast<int>(image.width()*scale);
	int height = static_cast<int>(image.height()*scale);

	tmpImg = image.scaled(width, height);
	setCurImageSize(width, height);
	setImageSize(tmpImg.width(), tmpImg.height());
	imgLabel->setPixmap(QPixmap::fromImage(tmpImg));
	imgLabel->resize(tmpImg.width(), tmpImg.height());
}

void DiyScrollArea::loadImage(QString &imgName)
{
	int dx = 0;
	int dy = 0;
	QImage *tmp = new QImage;

	if (!tmp->load(imgName))
	{
		QMessageBox::information(this, tr("Loading Error"),
			tr("Image loading error occurred!"));
	}
	image = *tmp;

	setImageSize(image.width(), image.height());
	imgLabel->setPixmap(QPixmap::fromImage(image));
	imgLabel->resize(image.width(), image.height());

	delete tmp;
}

void DiyScrollArea::dragEnterEvent(QDragEnterEvent *devent)
{
	devent->acceptProposedAction();
}

void DiyScrollArea::dropEvent(QDropEvent *devent)
{
	QList<QUrl> urls = devent->mimeData()->urls();

	if (urls.isEmpty())
	{
		QMessageBox::information(this, tr("Warning"), tr("An invalidate image!"));
	}
	QString path = urls.first().toLocalFile();

	loadImage(path);
}

void DiyScrollArea::keyPressEvent(QKeyEvent *kevent)
{
	if ((kevent->modifiers() == Qt::ControlModifier) &&
		(kevent->key() == Qt::Key_A))
	{
		scale > 1.0 ? scale = 1.0 : scale += 0.02;

		changeImgSize(scale + 1.0);
	}
	else if ((kevent->modifiers() == Qt::ControlModifier) &&
		(kevent->key() == Qt::Key_Z))
	{
		scale <= -0.96 ? scale = -0.96 : scale -= 0.02;

		changeImgSize(scale + 1.0);
	}
}