#include "threadinfo.h"

ThreadInfo::ThreadInfo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	activeBrush.setColor(QColor(255, 0, 0, 0));
	tableBrush.push_back(QBrush(QColor(85, 255, 127, 255)));
	tableBrush.push_back(QBrush(QColor(246, 255, 215, 255)));
	tableBrush.push_back(QBrush(QColor(213, 234, 255, 255)));
	tableBrush.push_back(QBrush(QColor(167, 166, 168, 255)));

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->verticalHeader()->setVisible(false);

	createCoordinate();
	resetAxisStat();
}

ThreadInfo::~ThreadInfo()
{

}

void ThreadInfo::processAxisStat(long* stat)
{
	int tmp = 0;

	for (int i = 0; i < 4; i++)
	{
		axisStat[i+1] = stat[i];
	}

	changeAxisStat();
}

void ThreadInfo::createCoordinate()
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

QString ThreadInfo::convertToString(int x, int y)
{
	QString strX, strY;

	strX.setNum(x);
	strY.setNum(y);
	strX = strX + " ";

	return strX + strY;
}

void ThreadInfo::changeAxisStat()
{
	QTableWidgetItem *item;
	QString eleCoord;

	for (int i = 0; i < 4; i++)
	{
		//servo enable
		eleCoord = convertToString(i, 1);
		item = itemCoordinate[eleCoord];
		if (axisStat[i + 1].test(9))
			item->setBackground(QBrush(QColor(255, 0, 0, 255)));
		else
			item->setBackground(tableBrush[i]);
		//motor in place
		eleCoord = convertToString(i, 2);
		item = itemCoordinate[eleCoord];
		if (axisStat[i + 1].test(10))
			item->setBackground(QBrush(QColor(255, 0, 0, 255)));
		else
			item->setBackground(tableBrush[i]);
		//drive alert
		eleCoord = convertToString(i, 3);
		item = itemCoordinate[eleCoord];
		if (axisStat[i+1].test(1))
			item->setBackground(QBrush(QColor(255, 0, 0, 255)));
		else
			item->setBackground(tableBrush[i]);
		//positive limit
		eleCoord = convertToString(i, 4);
		item = itemCoordinate[eleCoord];
		if (axisStat[i + 1].test(5))
			item->setBackground(QBrush(QColor(255, 0, 0, 255)));
		else
			item->setBackground(tableBrush[i]);
		//negative limit
		eleCoord = convertToString(i, 5);
		item = itemCoordinate[eleCoord];
		if (axisStat[i + 1].test(6))
			item->setBackground(QBrush(QColor(255, 0, 0, 255)));
		else
			item->setBackground(tableBrush[i]);
		//smooth stop
		eleCoord = convertToString(i, 6);
		item = itemCoordinate[eleCoord];
		if (axisStat[i + 1].test(7))
			item->setBackground(QBrush(QColor(255, 0, 0, 255)));
		else
			item->setBackground(tableBrush[i]);
		//urgent stop
		eleCoord = convertToString(i, 7);
		item = itemCoordinate[eleCoord];
		if (axisStat[i + 1].test(8))
			item->setBackground(QBrush(QColor(255, 0, 0, 255)));
		else
			item->setBackground(tableBrush[i]);
	}
}

void ThreadInfo::resetAxisStat()
{
	QTableWidgetItem *item;
	QString eleCoord;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 1; j <= 5; ++j)
		{
			eleCoord = convertToString(i, j);
			item = itemCoordinate[eleCoord];
			item->setBackground(tableBrush[i]);
		}
	}
}


