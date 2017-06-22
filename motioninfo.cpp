#include "motioninfo.h"

MotionInfo::MotionInfo(QObject *parent)
	: QObject(parent)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			axisMotionInfo[i][j] = 0;
		}
	}
}

MotionInfo::~MotionInfo()
{

}
