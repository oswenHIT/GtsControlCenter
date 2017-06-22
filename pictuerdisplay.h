#ifndef PICTUERDISPLAY_H
#define PICTUERDISPLAY_H

#include <QWidget>
#include "ui_pictuerdisplay.h"

class PictuerDisplay : public QWidget
{
	Q_OBJECT

public:
	PictuerDisplay(QWidget *parent = 0);
	~PictuerDisplay();

private:
	Ui::PictuerDisplay ui;
};

#endif // PICTUERDISPLAY_H
