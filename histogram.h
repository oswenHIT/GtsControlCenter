#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include "ui_histogram.h"

class Histogram : public QWidget
{
	Q_OBJECT

public:
	Histogram(QWidget *parent = 0);
	~Histogram();

private:
	Ui::Histogram ui;
};

#endif // HISTOGRAM_H
