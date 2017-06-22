#ifndef OVENTIMER_H
#define OVENTIMER_H

#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qfont.h>
#include <qpen.h>

class OvenTimer : public QWidget
{
	Q_OBJECT
		
public:
	OvenTimer(QWidget *parent = 0);

	void setDuration(int secs);
	int duration() const;
	void draw(QPainter *painter);

signals:
	void timeout();

protected:
	void paintEvent(QPaintEvent *pevent);
	void mousePressEvent(QMouseEvent *mevent);

private:
	QDateTime finishTime;
	QTimer *updateTimer;
	QTimer *finishTimer;
};


#endif