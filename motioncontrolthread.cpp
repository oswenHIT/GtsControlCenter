#include "motioncontrolthread.h"

MotionControlThread::MotionControlThread()
{
	stopped = false;
}

MotionControlThread::~MotionControlThread()
{

}

void MotionControlThread::run()
{
	while (!stopped)
	{
		qDebug() << messageStr;
	}

	stopped = false;
}

void MotionControlThread::stop()
{
	stopped = true;
}

void MotionControlThread::setMessage(const QString &str)
{
	messageStr = str;
}

