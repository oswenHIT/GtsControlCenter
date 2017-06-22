#include "diycircularslider.h"

DiyCircularSlider::DiyCircularSlider(QWidget *parent)
	: QSlider(parent)
{
	this->setPageStep(1);
	this->setSingleStep(1);
}

DiyCircularSlider::~DiyCircularSlider()
{

}

void DiyCircularSlider::wheelEvent(QWheelEvent *wevent)
{
	int curValue = this->value();
	int max = this->maximum();
	int min = this->minimum();

	if (curValue == max)
		this->setValue(min);
	else if (curValue == min)
	{
		this->setValue(max);
	}

	return QSlider::wheelEvent(wevent);
}