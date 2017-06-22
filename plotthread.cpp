#include "plotthread.h"

#define PI 3.1415

PlotThread::PlotThread(QCustomPlot *plot, QObject *parent)
	: QThread(parent)
{
	cnt = 0;
	stopped = false;
	customPlot = plot;
}

PlotThread::~PlotThread()
{

}

void PlotThread::run()
{
	double x;
	stopped = false;
	while (!stopped)
	{
		QMutexLocker locker(&mutex);
		xData << cnt++;
		yData << 10 * std::sin(cnt*PI / 120);
		if (xData.size() == 24000)
		{
			xData.clear();
			yData.clear();
		}
		x = xData.length() + 30;
		customPlot->graph(0)->setData(xData, yData);
		customPlot->xAxis->setRange(
			x, 
			customPlot->xAxis->range().size(), 
			Qt::AlignCenter);
		emit updatePlot();
		msleep(33);
	}
}

void PlotThread::stop()
{
	stopped = true;
}

void PlotThread::delayMs(int msecs)
{
	QDateTime last = QDateTime::currentDateTime();
	QDateTime now;
	while (1)
	{
		now = QDateTime::currentDateTime();
		if (last.msecsTo(now) >= msecs)
		{
			break;
		}
	}
}