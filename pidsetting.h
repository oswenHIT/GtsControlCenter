#ifndef PIDSETTING_H
#define PIDSETTING_H

#include <QWidget>
#include "ui_pidsetting.h"

class PIDSetting : public QWidget
{
	Q_OBJECT

public:
	PIDSetting(QWidget *parent = 0);
	~PIDSetting();

private:
	Ui::PIDSetting ui;
};

#endif // PIDSETTING_H
