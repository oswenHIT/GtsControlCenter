#include "axisstatmonitor.h"
#include "axisoperate.h"

AxisStatMonitor::AxisStatMonitor(AxisOperate *opt, QObject *parent)
	: QThread(parent), axisOpt(opt)
{
	stopped = false;
	info = new MotionInfo;
}

AxisStatMonitor::~AxisStatMonitor()
{

}

void AxisStatMonitor::run()
{
	stopped = false;
	int flag;

	while (!stopped)
	{
		msleep(70);

		getAxisStatInfo();
		getAxisMotionInfo();
	}
}

void AxisStatMonitor::stop()
{
	stopped = true;
}

void AxisStatMonitor::delayMs(int msec)
{
	QDateTime last = QDateTime::currentDateTime();
	QDateTime now;
	while (1)
	{
		now = QDateTime::currentDateTime();
		if (last.msecsTo(now) >= msec)
		{
			break;
		}
	}
}

void AxisStatMonitor::getAxisStatInfo()
{
	int flag;

	for (int i = 0; i < 4; i++)
	{
		flag = GT_GetSts(i + 1, axisStat + i);
		if (flag != 0)
		{
			emit sendError("GT_GetSts", i + 1, flag);
			return;
		}
	}

	emit sendAxisStat(axisStat);
}

void AxisStatMonitor::getAxisMotionInfo()
{
	int flag;

	for (int i = 0; i < 4; ++i)
	{
		long mode = 0;
		flag = GT_GetPrfMode(i + 1, &mode);
		if (flag != 0)
		{
			emit sendError("GT_GetPrfMode", i + 1, flag);
			return;
		}
		info->axisMotionInfo[i][0] = static_cast<double>(mode);
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 1; j < 8; ++j)
		{
			switch (j)
			{
			case 1:
				flag = GT_GetAxisEncPos(i + 1, info->axisMotionInfo[i] + 1);
				if (flag != 0)
				{
					emit sendError("GT_GetAxisEncPos", i + 1, flag);
					return;
				}
				break;

			case 2:
				flag = GT_GetPrfPos(i + 1, info->axisMotionInfo[i] + 2);
				if (flag != 0)
				{
					emit sendError("GT_GetPrfPos", i + 1, flag);
					return;
				}
				break;

			case 3:
				flag = GT_GetAxisError(i + 1, info->axisMotionInfo[i] + 3);
				if (flag != 0)
				{
					emit sendError("GT_GetAxisError", i + 1, flag);
					return;
				}
				break;

			case 4:
				flag = GT_GetAxisEncVel(i + 1, info->axisMotionInfo[i] + 4);
				if (flag != 0)
				{
					emit sendError("GT_GetAxisEncVel", i + 1, flag);
					return;
				}
				break;

			case 5:
				flag = GT_GetPrfVel(i + 1, info->axisMotionInfo[i] + 5);
				if (flag != 0)
				{
					emit sendError("GT_GetPrfVel", i + 1, flag);
					return;
				}
				break;

			case 6:
				flag = GT_GetAxisEncAcc(i + 1, info->axisMotionInfo[i] + 6);
				if (flag != 0)
				{
					emit sendError("GT_GetAxisEncAcc", i + 1, flag);
					return;
				}
				break;

			case 7:
				flag = GT_GetPrfAcc(i + 1, info->axisMotionInfo[i] + 7);
				if (flag != 0)
				{
					emit sendError("GT_GetPrfAcc", i + 1, flag);
					return;
				}
				break;

			default: break;
			}
		}
	}

	emit sendAxisMotionInfo(info);
}