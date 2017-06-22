#ifndef MOTIONMODE_H
#define MOTIONMODE_H

#include <QWidget>
#include <Qvector>
#include <qmainwindow.h>
#include <qmdiarea.h>
#include <qdebug.h>
#include <qcombobox.h>
#include <qmdisubwindow.h>
#include <map>
#include <vector>
#include <bitset>
#include "ui_motionmode.h"
#include "axisoperate.h"
#include "jogopt.h"
#include "pointposopt.h"
#include "pidsetting.h"
#include "tomotionmodedata.h"
#include "motioncontrol.h"
#include "plotwidget.h"

class MotionMode : public QWidget
{
	Q_OBJECT

public:
	MotionMode(AxisOperate *operate, QMdiArea *mdi = 0, QMainWindow *window = 0);
	~MotionMode();

	void initTable();
	void buildSignalSlot();
	void setTableWidget();
	void createJogCoordinate();
	void createPPCoordinate();
	void createPIDCoordinate();
	void displayJogModeInfo();
	void getJogData();
	void displayPPModeInfo();
	void getPPData();
	void enableMotionButton();
	QString doubleToString(double val);
	QString coorToString(int x, int y);
	void settingError(int axis);

public slots:
	void gtsCommandHandler(QString optName, int axis, int flag);
	void receiveModeData(toMotionModeData *data);
	void enableMotionMode(int x, int y);
	void enableButton();
	void disableButton();
	void loadJogSetting();
	void loadPPSetting();
	void jogStartButton();
	void jogStopButton();
	void startMotion();
	void gtsClosed();
	void threadFinished();
	void stopPlot();
	void startPlot();

signals:
	void sendJogParameter(int axis, JogModeVariable *var, toMotionModeData *data);
	void sendPPParameter(int axis, PPModeVariable *var, toMotionModeData *data);
	void sendSelectedAxis(toMotionModeData *data);
	void jogModeActivated();
	void jogModeClosed();
	void ppModeActivated();
	void ppModeClosed();
	void jogMotionDirec(bool direc);
	void trapActivated();
	void trapStart();
	void infoStart();
	void infoEnd();
	void hideAxisMode();
	void stopPlotSignal();
	void startPlotSignal();

private:
	Ui::MotionMode ui;
	QMainWindow *mainWindow;
	QMdiArea *mdiArea;
	AxisOperate *axisOperate;
	JogOpt *jogOpt;
	PointPosOpt *ppOpt;
	PIDSetting *pidSetting;
	MotionControl *motionControl;
	QTableWidgetItem* jogItem;
	QTableWidgetItem* ppItem;
	QMdiSubWindow *mdiWindow;

	bool isGtsOpen;
	bool jogDirec;
	bool foreOrBack;
	toMotionModeData *motionData;
	std::bitset<32> axisStat;
	std::map<QString, QTableWidgetItem*> jogParCoordinate;
	std::map<QString, QTableWidgetItem*> ppParCoordinate;
	std::map<QString, QTableWidgetItem*> pidParCoordinate;
	std::map<int, JogModeVariable*> jogModeVariable;
	std::map<int, PPModeVariable*> ppModeVariable;
	std::map<int, PIDParameter*> pidParameter;
	PIDParameter pidAxis1;
	PIDParameter pidAxis2;
	PIDParameter pidAxis3;
	PIDParameter pidAxis4;
	JogModeVariable axis1;
	JogModeVariable axis2;
	JogModeVariable axis3;
	JogModeVariable axis4;
	PPModeVariable ppAxis1;
	PPModeVariable ppAxis2;
	PPModeVariable ppAxis3;
	PPModeVariable ppAxis4;
};

#endif // MOTIONMODE_H
