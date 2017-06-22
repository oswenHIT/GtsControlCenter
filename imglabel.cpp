#include "imglabel.h"
#include <type_traits>

ImgLabel::ImgLabel(QWidget *parent)
	: QLabel(parent), isGrabMode(false)
{
	originalX = originalY = 0;
}

ImgLabel::~ImgLabel()
{

}

const wr::ROI & ImgLabel::getROI() const
{
	return roi;
}

wr::ROI & ImgLabel::getROI()
{
	return roi;
}

void ImgLabel::setListener(CListener * pLsn)
{
	pListener = pLsn;
}

void ImgLabel::mousePressEvent(QMouseEvent * ev)
{
	roi.isPress = true;
	roi.isRelease = false;

	originalX = roi.rect.right = roi.rect.left = ev->pos().rx();
	originalY = roi.rect.bottom = roi.rect.top = ev->pos().ry();
}

void ImgLabel::mouseMoveEvent(QMouseEvent * ev)
{
	auto x = ev->pos().rx();
	auto y = ev->pos().ry();

	int area;
	if (x >= 0 && x <= 1279 && y >= 0 && y <= 959)
	{
		if (originalX >= x && originalY < y)
		{
			roi.rect.left = x;
			roi.rect.bottom = y;
		}
		else if (originalX < x && originalY <= y)
		{
			roi.rect.right = x;
			roi.rect.bottom = y;
		}
		else if (originalX >= x && originalY > y)
		{
			roi.rect.left = x;
			roi.rect.top = y;
		}
		else if (originalX < x && originalY >= y)
		{
			roi.rect.right = x;
			roi.rect.top = y;
		}
	}

	pListener->setROI(roi);
}

void ImgLabel::mouseReleaseEvent(QMouseEvent * ev)
{
	if (isGrabMode)
		roi.isPress = false;

	pListener->setROI(roi);
}

void ImgLabel::setGrabMode(bool mode)
{
	roi.isGrabMode = mode;
}

void ImgLabel::setGrabber(DShowLib::Grabber * grab)
{
	grabber = grab;
}
