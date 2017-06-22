#ifndef POINTPOSOPT_H
#define POINTPOSOPT_H

#include <QWidget>
#include "ui_pointposopt.h"

class PointPosOpt : public QWidget
{
	Q_OBJECT

public:
	PointPosOpt(QWidget *parent = 0);
	~PointPosOpt();
	Ui::PointPosOpt ui;

private:
	
};

#endif // POINTPOSOPT_H
