#ifndef MOTIONINFO_H
#define MOTIONINFO_H

#include <QObject>

class MotionInfo : public QObject
{
	Q_OBJECT

public:
	MotionInfo(QObject *parent = 0);
	~MotionInfo();

	double axisMotionInfo[4][8];

private:

};

#endif // MOTIONINFO_H
