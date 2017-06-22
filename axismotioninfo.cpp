#include "axismotioninfo.h"

AxisMotionInfo::AxisMotionInfo(QWidget *parent)
	: QWidget(parent)
{
	isShowAxisMode = false;

	ui.setupUi(this);

	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		motionStat.insert(std::make_pair(i, std::vector<double>()));
	}

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->verticalHeader()->setVisible(false);

	createCoordinate();
	resetAxisMotionInfo();
}

AxisMotionInfo::~AxisMotionInfo()
{

}

void AxisMotionInfo::createCoordinate()
{
	QTableWidgetItem *item;
	QString ele;

	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		for (int j = 1; j < ui.tableWidget->columnCount(); j++)
		{
			item = ui.tableWidget->item(i, j);
			ele = convertToString(i, j);

			if (item != 0)
			{
				itemCoordinate.insert(std::make_pair(ele, item));
			}
		}
	}
}

void AxisMotionInfo::processAxisMotionInfo(MotionInfo *motionInfo)
{
	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		motionStat[i].clear();
		for (int j = 0; j < ui.tableWidget->columnCount()-1; ++j)
		{
			motionStat[i].push_back(motionInfo->axisMotionInfo[i][j]);
		}
	}

	updateMotionInfo();
}

void AxisMotionInfo::changeMotionStat()
{
}

void AxisMotionInfo::updateMotionInfo()
{
	QTableWidgetItem *item;
	QString eleCoord;
	QString content;

	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		for (int j = 1; j < ui.tableWidget->columnCount(); ++j)
		{
			eleCoord = convertToString(i, j);
			item = itemCoordinate[eleCoord];
			if (item != 0)
			{
				(j == 1) ?
					content = motionMode(motionStat[i][j - 1]) :
					content = doubleToString(motionStat[i][j - 1]);
				item->setText(content);
			}
		}
	}
}

void AxisMotionInfo::resetAxisMotionInfo()
{
	QTableWidgetItem *item;
	QString eleCoord;
	QString content;

	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		for (int j = 1; j < ui.tableWidget->columnCount(); ++j)
		{
			eleCoord = convertToString(i, j);
			item = itemCoordinate[eleCoord];
			if (item != 0)
			{
				(j == 1) ?
					content = "---" :
					content = "0.00";

				item->setText(content);
			}
		}
	}
}

QString AxisMotionInfo::convertToString(int x, int y)
{
	QString strX, strY;

	strX.setNum(x);
	strY.setNum(y);
	strX = strX + " ";

	return strX + strY;
}

QString AxisMotionInfo::doubleToString(double num)
{
	QString str;
	str.setNum(num, 'f', 2);

	return str;
}

void AxisMotionInfo::showAxisMode()
{
	isShowAxisMode = true;
}

void AxisMotionInfo::hideAxisMode()
{
	isShowAxisMode = false;
}

QString AxisMotionInfo::motionMode(double m)
{
	QString str;

	if (isShowAxisMode)
	{
		long mode = static_cast<long>(m);
		switch (mode)
		{
		case 0: str = tr("Trap Mode"); break;
		case 1: str = tr("Jog Mode"); break;
		case 2: str = tr("P-T Mode"); break;
		case 3: str = tr("Gear Mode"); break;
		case 4: str = tr("Follow Mode"); break;
		case 5: str = tr("Interpolate"); break;
		case 6: str = tr("Pvt Mode"); break;
		default: str = tr("Axis Error"); break;
		}
	}
	else
	{
		str = "---";
	}

	return str;
}