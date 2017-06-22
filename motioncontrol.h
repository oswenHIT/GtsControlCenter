#ifndef MOTIONCONTROL_H
#define MOTIONCONTROL_H

#include <qalgorithms.h>
#include <QThread>
#include <gts.h>
#include <qdebug.h>
#include <vector>
#include <bitset>
#include <qdatetime.h>
#include <qmessagebox.h>
#include <qsemaphore.h>
#include <qreadwritelock.h>
#include "axismotionthread.h"
#include "tomotionmodedata.h"

#define JogMode 1
#define TrapMode 2

class MotionControl : public QThread
{
	Q_OBJECT

public:
	void writeMotionInfo(int axis);
	void delayMs(int msec);
	int abs(double val1, double val2);

	MotionControl(QWidget *widget = 0, QObject *parent = 0);
	~MotionControl();


public slots:
	void setJogParameter(int axis, JogModeVariable *var, toMotionModeData *data);
	void setPPParameter(int axis, PPModeVariable *var, toMotionModeData *data);
	void activateJogMode();
	void disableJogMode();
	void activatePPMode();
	void disablePPMode();
	void motionStart();
	void jogMove(bool direc);
	void infoBegin();
	void infoEnd();
	void stop();

signals:
	void gtsCommandHandler(QString optName, int axis, int flag);
	void showAxisMode();
	void startJogMotion();
	void jogDirection(bool direc);

protected:
	void run();

private:
	QWidget *parentWidget;

	int singleAxis;
	int cnt = 1;
	bool jogMode;
	bool ppMode;
	bool stopped;
	bool jogDirec;
	bool beginOfInfo;
	TJogPrm *jogPrm;
	AxisMotionThread *axisOne;
	AxisMotionThread *axisTwo;
	AxisMotionThread *axisThree;
	AxisMotionThread *axisFour;
	std::map<int, AxisMotionThread*> axisMotion;
	std::map<int, JogModeVariable> jogSetInfo;
	std::map<int, PPModeVariable> ppSetInfo;
	std::map<int, int> trapCircleTimes;
	std::vector<int> curAxis;
	std::bitset<32> axisStat;
	enum {None, Jog, Trap}mode;
};

#endif // MOTIONCONTROL_H
