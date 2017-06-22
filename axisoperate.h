#ifndef AXISOPERATE_H
#define AXISOPERATE_H

#include <bitset>
#include <QWidget>
#include <qmessagebox.h>
#include <qthread.h>
#include <gts.h>
#include <map>
#include "ui_axisoperate.h"
#include "axisstatmonitor.h"
#include "motioncontrol.h"
#include "tomotionmodedata.h"

class AxisOperate : public QWidget
{
	Q_OBJECT

public:
	bool isGtsOpen() { return gtsOpenFlag; }
	void initGts();
	void buildSignalSlot();

	AxisOperate(QWidget *parent = 0);
	~AxisOperate();

public slots:
	void gtsOpen();
	void gtsStop();
	void gtsReset();
	void aboutGTS_400();
	void processAxisStat(long* stat); 
	void processAxisMotionInfo(MotionInfo *motionInfo);
	void gtsCommandHandler(QString cmdName, int axis, int flag);
	void statClear();
	void posClear();
	void servoEnable();
	void axisStop();
	void changeCurAxisNum(int axis);
	void axisOneCheck(bool checked);
	void axisTwoCheck(bool checked);
	void axisThreeCheck(bool checked);
	void axisFourCheck(bool checked);
	void disableAxisCheckStat(int axis);
	void createModeData(int val);

signals:
	void sendAxisStat(long* stat);
	void sendAxisMotionInfo(MotionInfo *motionInfo);
	void gtsHasClosed();
	void gtsHasOpened();
	void singleModeStat(bool check);
	void sendModeData(toMotionModeData *data);
	void showAxisMode();
	void hideAxisMode();

private:
	Ui::AxisOperate ui;
	AxisStatMonitor *axisStatThread;

	bool gtsOpenFlag;
	long axisStat[4];
	int curAxisNum;
	QString gtsVersion;
	MotionInfo *info;
	std::map<int, bool> axisSelected;
	std::map<int, QCheckBox*> correlateAxis;
	toMotionModeData axisData;
};

#endif AXISOPERATE_H