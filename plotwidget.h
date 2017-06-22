#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qpoint.h>
#include <qcustomplot.h>
#include <qvector.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <qmath.h>
#include <qevent.h>
#include <qmutex.h>
#include <qfiledialog.h>
#include <map>
#include "tomotionmodedata.h"
#include "ui_plotwidget.h"

class PlotWidget : public QWidget
{
	Q_OBJECT

public:
	void initCustomPlot1();
	void initCustomPlot2();
	void initCustomPlot3();
	void initCustomPlot4();
	void initFileDialog();
	int determineAxis();

	PlotWidget(QMainWindow *window = 0, QWidget *widget = 0);
	~PlotWidget();

public slots:
	void showInfo(QPoint pos, int axis);
	void updatePlot();
	void stopPlot();
	void startPlot();
	void savePicture();
	void trackModeEnable();
	void axisOneVisible();
	void axisTwoVisible();
	void axisThreeVisible();
	void axisFourVisible();
	void clearData();
	void receiveModeData(toMotionModeData *data);

protected:
	bool eventFilter(QObject *obj, QEvent *ev);
	
private:
	Ui::PlotWidget ui;
	QMainWindow *mainWindow;
	QTimer *timer;
	QCustomPlot* plotMap[4];
	QCPItemText* text[4];
	QCPItemTracer *tracer[12];
	QFileDialog *saveFile;

	int curXAxis;
	int curPos, curSize;
	bool axisDblClc[4];
	bool zoomMode[4];
	bool trackMode;
	QString mode[4];
	QVector<double> xData;
	QVector<double> prfvel[4];
	QVector<double> encvel[4];
	QVector<int> axisQueue;
	QVector<int>::const_iterator axisIter;
	toMotionModeData *axisData;
};

#endif // PLOTWIDGET_H
