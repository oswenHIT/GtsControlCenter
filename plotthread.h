#ifndef PLOTTHREAD_H
#define PLOTTHREAD_H

#include <QThread>
#include <qcustomplot.h>
#include <qvector.h>
#include <qdatetime.h>
#include <qmutex.h>

class PlotThread : public QThread
{
	Q_OBJECT

public:
	void stop();
	void delayMs(int msecs);

	PlotThread(QCustomPlot *plot, QObject *parent = 0);
	~PlotThread();

signals:
	void updatePlot();

protected:
	void run();

private:
	int cnt;
	bool stopped;
	QVector<double> xData;
	QVector<double> yData;
	QCustomPlot *customPlot;
	QMutex mutex;
};

#endif // PLOTTHREAD_H
