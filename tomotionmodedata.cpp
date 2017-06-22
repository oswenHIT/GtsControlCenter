#include "tomotionmodedata.h"

toMotionModeData::toMotionModeData(QObject *parent)
	: QObject(parent)
{
	curAxisNum = 1;
	correlateAxis.push_back(0);
}

toMotionModeData::~toMotionModeData()
{

}
