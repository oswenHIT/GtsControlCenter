#ifndef TOMOTIONMODEDATA_H
#define TOMOTIONMODEDATA_H

#include <QObject>
#include <qcheckbox.h>
#include <map>
#include <vector>
#include <qvector.h>

class toMotionModeData : public QObject
{
	Q_OBJECT

public:
	toMotionModeData(QObject *parent = 0);
	~toMotionModeData();

	int curAxisNum;
	QVector<int> correlateAxis;

private:
	
};

class JogModeVariable : public QObject
{
	Q_OBJECT

public:
	double acc;
	double dec;
	double smooth;
	double velocity;

	JogModeVariable(){ acc = 0.0; dec = 0.0; smooth = 0.0; velocity = 0.0; }
};

class PPModeVariable : public QObject
{
	Q_OBJECT

public:
	double acc;
	double dec;
	double velocity;
	double velStart;
	short smooth;
	short delayTm;
	long length;
	int circles;

	PPModeVariable()
	{
		acc = 0.0; 
		dec = 0.0; 
		velocity = 0.0;
		velStart = 0.0;
		smooth = 0; 
		delayTm = 0;
		length = 0;
		circles = 0;
	}
};

class PIDParameter : public QObject
{
	Q_OBJECT

public:
	double kp;
	double ki;
	double kd;
	double kvff;
	double kaff;
	long integralLimit;
	long derivativeLimit;
	short limit;

	PIDParameter()
	{
		kp = 0.0;
		ki = 0.0;
		kd = 0.0;
		kvff = 0.0;
		kaff = 0.0;
		integralLimit = 0;
		derivativeLimit = 0;
		limit = 0;
	}
};

#endif // TOMOTIONMODEDATA_H
