#ifndef THREADINFO_H
#define THREADINFO_H

#include <QWidget>
#include <qlabel.h>
#include <bitset>
#include <map>
#include <vector>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qbrush.h>
#include <qcolor.h>
#include "ui_threadinfo.h"

class ThreadInfo : public QWidget
{
	Q_OBJECT

public:
	void createCoordinate();
	void changeAxisStat();
	QString convertToString(int x, int y);

	ThreadInfo(QWidget *parent = 0);
	~ThreadInfo();
	QSize sizeHint() const
	{
		return QSize(602, 147);
	}

public slots:
	void processAxisStat(long* stat);
	void resetAxisStat();

signals:
	void axisDisabled(int axis);

private:
	Ui::ThreadInfo ui;
	std::map<int, std::bitset<32> > axisStat;
	std::map<QString, QTableWidgetItem*> itemCoordinate;
	std::vector<QBrush> tableBrush;
	QBrush activeBrush;
};

#endif //THREADINFO_H
