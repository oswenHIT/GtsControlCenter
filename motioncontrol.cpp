#include "motioncontrol.h"

QVector<double> xdata;
QVector<double> prfVel[4];
QVector<double> encVel[4];
QVector<double> posError[4];
QReadWriteLock mutex;

MotionControl::MotionControl(QWidget *widget, QObject *parent)
	: QThread(parent)
{
	parentWidget = widget;

	singleAxis = 0;
	stopped = false;
	jogMode = false;
	ppMode = false;
	beginOfInfo = false;
	jogDirec = false;

	axisOne = new AxisMotionThread(parentWidget);
	axisMotion[1] = axisOne;
	axisOne->setCurAxis(1);
	axisTwo = new AxisMotionThread(parentWidget);
	axisMotion[2] = axisTwo;
	axisTwo->setCurAxis(2);
	axisThree = new AxisMotionThread(parentWidget);
	axisMotion[3] = axisThree;
	axisThree->setCurAxis(3);
	axisFour = new AxisMotionThread(parentWidget);
	axisMotion[4] = axisFour;
	axisFour->setCurAxis(4);

	connect(this, &MotionControl::jogDirection, axisOne, &AxisMotionThread::receiveJogDirec);
	connect(this, &MotionControl::jogDirection, axisTwo, &AxisMotionThread::receiveJogDirec);
	connect(this, &MotionControl::jogDirection, axisThree, &AxisMotionThread::receiveJogDirec);
	connect(this, &MotionControl::jogDirection, axisFour, &AxisMotionThread::receiveJogDirec);
}

MotionControl::~MotionControl()
{

}

void MotionControl::run()
{
	int axis, flag, cnt = 0;
	unsigned long time = 0;
	
	stopped = false;
	while (!stopped)
	{
		mutex.lockForWrite();
		GT_GetClock(&time);
		xdata.append(time);
		if (xdata.size() == 3000)
		{
			xdata.remove(0, 2000);
			for (int i = 0; i < curAxis.size(); ++i)
			{
				int index = curAxis[i];
				prfVel[index - 1].remove(0, 2000);
				encVel[index - 1].remove(0, 2000);
			}
		}
		mutex.unlock();

		for (int i = 0; i < curAxis.size(); ++i)
		{
			axis = curAxis[i];
			writeMotionInfo(axis);
		}
		
		msleep(5);
	}
}

void MotionControl::writeMotionInfo(int axis)
{
	int flag;
	double pV, eV, error;
	unsigned long time;

	mutex.lockForWrite();
	flag = GT_GetPrfVel(axis, &pV, 1, &time);
	flag = GT_GetEncVel(axis, &eV);
	encVel[axis - 1].append(eV);
	prfVel[axis - 1].append(pV);
	mutex.unlock();
}

void MotionControl::stop()
{
	int axis, flag;
	for (int i = 0; i < curAxis.size(); ++i)
	{
		axis = curAxis[i];
		axisMotion[axis]->stop();
	}
	
	stopped = true;
}

void MotionControl::activateJogMode()
{
	jogMode = true;
	mode = Jog;
}

void MotionControl::disableJogMode()
{
	jogMode = false;
	mode = None;
}

void MotionControl::activatePPMode()
{
	ppMode = true;
	mode = Trap;
}

void MotionControl::disablePPMode()
{
	ppMode = false;
	mode = None;
}

void MotionControl::setJogParameter(
	int axis, JogModeVariable *var, toMotionModeData *data)
{
	if (jogMode)
	{
		curAxis.push_back(axis);

		var->acc == 0 ? jogSetInfo[axis].acc = 0.5 : jogSetInfo[axis].acc = var->acc;
		var->dec == 0 ? jogSetInfo[axis].dec = 0.5 : jogSetInfo[axis].dec = var->dec;
		var->smooth == 0 ? jogSetInfo[axis].smooth = 0.5 : jogSetInfo[axis].smooth = var->smooth;
		jogSetInfo[axis].velocity = var->velocity;

		axisMotion[axis]->setJogPar(&jogSetInfo[axis]);
		axisMotion[axis]->setMotionMode(JogMode);
	}
}

void MotionControl::jogMove(bool direc)
{
	jogDirec = true;

	emit jogDirection(direc);
	emit startJogMotion();
}

void MotionControl::setPPParameter(
	int axis, PPModeVariable *var, toMotionModeData *data)
{
	if (ppMode)
	{
		curAxis.push_back(axis);

		var->acc == 0 ? ppSetInfo[axis].acc = 0.5 : ppSetInfo[axis].acc = var->acc;
		var->dec == 0 ? ppSetInfo[axis].dec = 0.5 : ppSetInfo[axis].dec = var->dec;
		var->smooth == 0 ? ppSetInfo[axis].smooth = 25 : ppSetInfo[axis].smooth = var->smooth;
		
		ppSetInfo[axis].velStart = var->velStart;
		ppSetInfo[axis].length = var->length;
		ppSetInfo[axis].velocity = var->velocity;
		ppSetInfo[axis].velStart = var->velStart;
		ppSetInfo[axis].circles = 2 * var->circles;
		ppSetInfo[axis].delayTm = var->delayTm;

		axisMotion[axis]->setCurAxis(axis);
		axisMotion[axis]->setTrapPar(&ppSetInfo[axis]);
		axisMotion[axis]->setMotionMode(TrapMode);
	}
}

void MotionControl::infoBegin()
{
	curAxis.clear();
	beginOfInfo = true;
	emit showAxisMode();
}

void MotionControl::infoEnd()
{
	beginOfInfo = false;
}

void MotionControl::delayMs(int msec)
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

int MotionControl::abs(double val1, double val2)
{
	double val = val1 - val2;
	int diff = static_cast<int>(val);

	return diff > 0 ? diff : -diff;
}

void MotionControl::motionStart()
{
	int axis = 0;
	for (int i = 0; i < curAxis.size(); ++i)
	{
		axis = curAxis[i];
		axisMotion[axis]->start();
	}
}