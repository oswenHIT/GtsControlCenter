#include "diyspinbox.h"

DiySpinBox::DiySpinBox(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(changeColor()));
}

DiySpinBox::~DiySpinBox()
{

}

void DiySpinBox::changeColor()
{
/*	QRect rect(ui.pushButton->x(), ui.pushButton->y(), ui.pushButton->width(), ui.pushButton->height());
	QPainter painter(ui.pushButton);
	painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
	painter.setPen(QPen(Qt::blue, 15, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	painter.drawRect(rect);*/
	//ui.pushButton->setStyleSheet("color:blue");
}

void DiySpinBox::paintEvent(QPaintEvent *pevent)
{
//	QRect rect(ui.pushButton->x(), ui.pushButton->y(), ui.pushButton->width(), ui.pushButton->height());
//	QPainter painter(ui.pushButton);
	//painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
	//painter.setPen(QPen(Qt::blue, 15, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	//painter.drawRect(rect);

	QWidget::paintEvent(pevent);
}
