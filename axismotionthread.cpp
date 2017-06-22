#include "axismotionthread.h"

AxisMotionThread::AxisMotionThread(QWidget *widget, QObject *parent)
	: QThread(parent)
{
	stopped = false;
	jogDirection = false;
	curAxis = 0;
	trapCircles = 0;
	motionMode = NONE;
	parentWidget = widget;
}

AxisMotionThread::~AxisMotionThread()
{

}

void AxisMotionThread::setJogPar(JogModeVariable *var)
{
	int flag = 0;
	flag = GT_Stop(1 << (curAxis - 1), 0);
	gtsCommandHandler("GT_Stop", curAxis, flag);

	jogModeVar.acc = var->acc;
	jogModeVar.dec = var->dec;
	jogModeVar.velocity = var->velocity;
	jogModeVar.smooth = var->smooth;

	switchToJog();
}

void AxisMotionThread::setTrapPar(PPModeVariable *var)
{
	ppModeVar.acc = var->acc;
	ppModeVar.dec = var->dec;
	ppModeVar.smooth = var->smooth;
	ppModeVar.velocity = var->velocity;
	ppModeVar.delayTm = var->delayTm;
	ppModeVar.circles = var->circles;
	ppModeVar.length = var->length;
	ppModeVar.velStart = var->velStart;
	trapCircles = var->circles;

	switchToTrap();
}

void AxisMotionThread::switchToJog()
{
	int flag = 0, vel = 0;

	TJogPrm jog;
	jog.acc = jogModeVar.acc;
	jog.dec = jogModeVar.dec;
	jog.smooth = jogModeVar.smooth;

	flag = GT_ZeroPos(curAxis);
	gtsCommandHandler("GT_ZeroPos", curAxis, flag);
	flag = GT_PrfJog(curAxis);
	gtsCommandHandler("GT_PrfJog", curAxis, flag);
	flag = GT_SetJogPrm(curAxis, &jog);
	gtsCommandHandler("GT_SetJogPrm", curAxis, flag);
	vel = jogModeVar.velocity;
	flag = GT_SetVel(curAxis, vel);
	gtsCommandHandler("GT_SetVel", curAxis, flag);
}

void AxisMotionThread::switchToTrap()
{
	int flag = 0, vel;
	long length;
	TTrapPrm trap;

	trap.acc = ppModeVar.acc;
	trap.dec = ppModeVar.dec;
	trap.smoothTime = ppModeVar.smooth;
	trap.velStart = ppModeVar.velStart;
	
	flag = GT_ZeroPos(curAxis);
	gtsCommandHandler("GT_ZeroPos", curAxis, flag);
	flag = GT_PrfTrap(curAxis);
	gtsCommandHandler("GT_PrfTrap", curAxis, flag);
	flag = GT_SetTrapPrm(curAxis, &trap);
	gtsCommandHandler("GT_SetTrapPrm", curAxis, flag);
	length = ppModeVar.length;
	flag = GT_SetPos(curAxis, length);
	gtsCommandHandler("GT_SetPos", curAxis, flag);
	vel = ppModeVar.velocity;
	flag = GT_SetVel(curAxis, vel);
	gtsCommandHandler("GT_SetVel", curAxis, flag);
}

void AxisMotionThread::receiveJogDirec(bool direc)
{
	int velocity, flag;
	long stat;

	flag = GT_GetSts(curAxis, &stat);
	gtsCommandHandler("GT_GetSts", curAxis, flag);
	axisStat = stat;
	if (axisStat.test(9))
	{
		jogDirection = direc;
		velocity = jogModeVar.velocity;
		if (jogDirection)
			velocity = 0 - velocity;
		flag = GT_SetVel(curAxis, velocity);
		gtsCommandHandler("GT_SetVel", curAxis, flag);
		flag = GT_Update(1 << (curAxis - 1));
		gtsCommandHandler("GT_Update", curAxis, flag);
	}
}

void AxisMotionThread::stop()
{
	int flag = 0;
	stopped = true;
	flag = GT_Stop(1 << (curAxis - 1), 0);
}

void AxisMotionThread::run()
{
	int flag;
	double curLength ;
	long tmp = ppModeVar.length, length = 0;
	bool direc = true;

	flag = GT_Update(1 << (curAxis - 1));

	stopped = false;
	while (!stopped)
	{
		switch (motionMode)
		{
		case TrapMode:
			GT_GetPrfPos(curAxis, &curLength);
			if (curLength == tmp)
			{
				flag = GT_Stop(1 << (curAxis - 1), 0);
				gtsCommandHandler("GT_Stop", curAxis, flag);
				if (direc)
				{
					length = 0;
					direc = false;
				}
				else
				{
					length = ppModeVar.length;
					direc = true; 
				}
		
				tmp = length;
				flag = GT_SetPos(curAxis, length);
				gtsCommandHandler("GT_SetPos", curAxis, flag);
				flag = GT_Update(1 << (curAxis - 1));
				gtsCommandHandler("GT_Update", curAxis, flag);
				--trapCircles;
				if (trapCircles == 0)
				{
					this->stop();
				}
			}
			break;
		default:
			break;
		};
	}
}

void AxisMotionThread::gtsCommandHandler(QString optName, int axis, int flag)
{
	if (flag)
	{
		switch (flag)
		{
		case 1:
			QMessageBox::information(
				parentWidget,
				tr("Gts command error"),
				tr("Axis %1 %2 \nExecution of the instruction error").arg(axis).arg(optName));
			break;

		case 2:
			QMessageBox::information(
				parentWidget,
				tr("Gts command error"),
				tr("Axis %1 %2 \nGts Controller does not support this feature").arg(axis).arg(optName));
			break;

		case 7:
			QMessageBox::information(
				parentWidget,
				tr("Gts command error"),
				tr("Axis %1 %2 \nInstruction parameter error").arg(axis).arg(optName));
			break;

		case -1:
			QMessageBox::information(
				parentWidget,
				tr("Gts command error"),
				tr("Axis %1 %2 \nHost and motion controller communication failed").arg(axis).arg(optName));
			break;

		case -6:
			QMessageBox::information(
				parentWidget,
				tr("Gts command error"),
				tr("Axis %1 %2 \nFailed to open the controller").arg(axis).arg(optName));
			break;

		case -7:
			QMessageBox::information(
				parentWidget,
				tr("Gts command error"),
				tr("Axis %1 %2 \nMotion controller does not respond").arg(axis).arg(optName));
			break;

		default:
			break;
		}
	}
}