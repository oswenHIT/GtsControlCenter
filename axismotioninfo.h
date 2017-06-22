#ifndef AXISMOTIONINFO_H
#define AXISMOTIONINFO_H

#include <map>
#include <vector>
#include <qdebug.h>
#include "ui_axismotioninfo.h"
#include "motioninfo.h"

class AxisMotionStat{
public:
	long mode = 0;
	double planPos = 0;
	double planVelocity = 0;
	double planAcc = 0;
	double realPos = 0;
	double realVelocity = 0;
	double realAcc = 0;
	double posError = 0;
};

class AxisMotionInfo : public QWidget
{
	Q_OBJECT

public:
	void createCoordinate();
	void changeMotionStat();
	void updateMotionInfo();
	QString motionMode(double mode);
	QString convertToString(int x, int y);
	QString doubleToString(double num);

	AxisMotionInfo(QWidget *parent = 0);
	~AxisMotionInfo();

public slots:
	void processAxisMotionInfo(MotionInfo *motionInfo);
	void resetAxisMotionInfo();
	void showAxisMode();
	void hideAxisMode();

private:
	Ui::AxisMotionInfo ui;
	std::map<QString, QTableWidgetItem*> itemCoordinate;
	std::map<int, std::vector<double> > motionStat;

	bool isShowAxisMode;
};

#endif // AXISMOTIONINFO_H
