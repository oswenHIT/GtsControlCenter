#ifndef JOGOPT_H
#define JOGOPT_H

#include <QWidget>
#include "ui_jogopt.h"

class JogOpt : public QWidget
{
	Q_OBJECT

public:
	JogOpt(QWidget *parent = 0);
	~JogOpt();
	Ui::JogOpt ui;
private:	
};

#endif // JOGOPT_H
