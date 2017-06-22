#ifndef AXISSTATMONITOR_H
#define AXISSTATMONITOR_H

#include <QThread>
#include <bitset>
#include <qdatetime.h>
#include <gts.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include "motioninfo.h"

class AxisOperate;

class AxisStatMonitor : public QThread
{
	Q_OBJECT

public:
	AxisStatMonitor(AxisOperate *opt = 0, QObject *parent = 0);
	~AxisStatMonitor();
	void getAxisStatInfo();
	void getAxisMotionInfo();
	void delayMs(int msec);
	void stop();

signals:
	void sendAxisStat(long* stat);
	void sendAxisMotionInfo(MotionInfo *motionInfo);
	void sendError(QString cmdName, int axis, int flag);

protected:
	void run();

private:
	bool stopped;
	AxisOperate *axisOpt;

	long axisStat[4];
	MotionInfo *info;
};

#endif //AXISSTATMONITOR_H
