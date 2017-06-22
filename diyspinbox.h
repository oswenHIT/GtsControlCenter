#ifndef DIYSPINBOX_H
#define DIYSPINBOX_H

#include <QWidget>
#include <qpainter.h>
#include "ui_diyspinbox.h"

class DiySpinBox : public QWidget
{
	Q_OBJECT

public:
	DiySpinBox(QWidget *parent = 0);
	~DiySpinBox();

private slots:
	void changeColor();

protected:
	void paintEvent(QPaintEvent *pevent);

private:
	Ui::DiySpinBox ui;
};

#endif // DIYSPINBOX_H
