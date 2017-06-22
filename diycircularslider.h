#ifndef DIYCIRCULARSLIDER_H
#define DIYCIRCULARSLIDER_H

#include <QSlider>
#include <qevent.h>

class DiyCircularSlider : public QSlider
{
	Q_OBJECT

public:
	DiyCircularSlider(QWidget *parent = 0);
	~DiyCircularSlider();

protected:
	void wheelEvent(QWheelEvent *wevent);

private:
	
};

#endif // DIYCIRCULARSLIDER_H
