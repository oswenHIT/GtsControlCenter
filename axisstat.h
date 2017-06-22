#ifndef AXISSTAT_H
#define AXISSTAT_H

#include <QWidget>
#include "ui_axisstat.h"

class AxisStat : public QWidget
{
	Q_OBJECT

public:
	AxisStat(QWidget *parent = 0);
	~AxisStat();

private:
	Ui::AxisStat ui;
};

#endif // AXISSTAT_H
