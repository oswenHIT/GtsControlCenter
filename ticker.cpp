#include "ticker.h"

void Ticker::setText(const QString &newText)
{
	myText = newText;

	update();
	updateGeometry();
}

QSize Ticker::sizeHint() const
{
	return fontMetrics().size(0, text());
}

void Ticker::paintEvent(QPaintEvent *pevent)
{
	QPainter painter(this);

	int textWidth = fontMetrics().width(text());
	if (textWidth < 1)
	{
		return;
	}

	int x = -offset;
	while (x < width())
	{
		painter.drawText(x, 0, textWidth, height(), 
						 Qt::AlignCenter | Qt::AlignLeft, text());
		x += textWidth;
	}
}

void Ticker::showEvent(QShowEvent *sevent)
{
	myTimerID = startTimer(30);
}

void Ticker::timerEvent(QTimerEvent *tevent)
{
	if (tevent->timerId() == myTimerID)
	{
		++offset;
		if (offset >= fontMetrics().width(text()))
		{
			offset = 0;
		}
		scroll(-1, 0);
	}
	else
	{
		QWidget::timerEvent(tevent);
	}
}

void Ticker::hideEvent(QHideEvent *hevent)
{
	killTimer(myTimerID);
	myTimerID = 0;
}