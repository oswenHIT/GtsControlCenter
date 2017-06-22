#ifndef AXISMOTIONTHREAD_H
#define AXISMOTIONTHREAD_H

#include <QThread>
#include <gts.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <bitset>
#include "tomotionmodedata.h"

#define NONE 0
#define JogMode 1
#define TrapMode 2

class AxisMotionThread : public QThread
{
	Q_OBJECT

public:
	void setCurAxis(int axis) { curAxis = axis; qDebug() << "motionThread " << curAxis; }
	void setMotionMode(int mode){ motionMode = mode; }
	void setJogPar(JogModeVariable *var);
	void setTrapPar(PPModeVariable *var);
	void switchToJog();
	void switchToTrap();
	void gtsCommandHandler(QString cmdName, int axis, int flag);

	AxisMotionThread(QWidget *widget = 0, QObject *parent = 0);
	~AxisMotionThread();

public slots:
	void stop();
	void receiveJogDirec(bool direc);

protected:
	void run();

private:
	std::bitset<32> axisStat;
	bool stopped;
	bool jogDirection;
	int curAxis;
	int motionMode;
	int trapCircles;
	QWidget *parentWidget;
	JogModeVariable jogModeVar;
	PPModeVariable ppModeVar;
};

#endif // AXISMOTIONTHREAD_H
