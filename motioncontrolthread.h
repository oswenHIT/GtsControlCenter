#ifndef MOTIONCONTROLTHREAD_H
#define MOTIONCONTROLTHREAD_H

#include <QThread>
#include <qdialog.h>
#include <qevent.h>
#include <qdebug.h>
#include <qmutex.h>
#include <iostream>

class MotionControlThread : public QThread
{
	Q_OBJECT

public:
	MotionControlThread();
	~MotionControlThread();

	void setMessage(const QString &message);
	void stop();

protected:
	void run();

private:
	QMutex mutex;
	QString messageStr;
	volatile bool stopped;
};

#endif // MOTIONCONTROLTHREAD_H
