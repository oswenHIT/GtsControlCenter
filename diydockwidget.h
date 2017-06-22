#ifndef DIYDOCKWIDGET_H
#define DIYDOCKWIDGET_H

#include <QDockWidget>
#include <qevent.h>

class DiyDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	DiyDockWidget(QWidget *parent = 0);
	~DiyDockWidget();

protected:
	void closeEvent(QCloseEvent *cevent);

private:
	
};

#endif // DIYDOCKWIDGET_H
