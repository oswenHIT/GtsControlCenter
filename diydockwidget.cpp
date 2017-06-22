#include "diydockwidget.h"

DiyDockWidget::DiyDockWidget(QWidget *parent)
	: QDockWidget(parent)
{

}

DiyDockWidget::~DiyDockWidget()
{

}

void DiyDockWidget::closeEvent(QCloseEvent *cevent)
{
	this->hide();
}
