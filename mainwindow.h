#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>
#include <QMainWindow>
#include <QtWidgets>
#include <qwhatsthis.h>
#include <qprogressdialog.h>
#include <qtableview.h>
#include <qtablewidget.h>
#include <qevent.h>
#include <qfiledialog.h>
#include <QSpacerItem>
#include <qsettings.h>
#include <qmdiarea.h>
#include <qmdisubwindow.h>
#include <qdebug.h>
#include <qdir.h>
#include <map>
#include <gts.h>
#include "ui_mainwindow.h"
#include "axisoperate.h"
#include "imageproc.h"
#include "histogram.h"
#include "axispropertysheet.h"
#include "diyscrollarea.h"
#include "threadinfo.h"
#include "motionmode.h"
#include "selecteimage.h"
#include "axismotioninfo.h"
#include "diydockwidget.h"
#include "plotwidget.h"

class mdiData
{
public:
	bool flag;
	QMdiSubWindow *window;
	DiyScrollArea *scrollArea;

	mdiData() : flag(false), window(0), scrollArea(0) { }
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void createMainWidget();
	void writeSettings();
	void readSettings();
	void windowMenuClear();
	void createWhatsThis();
	void createLanguageMenu();
	void createViewMenu();
	void buildSignalSlot();

public slots:
	void aboutApp();
	void openPicture();
	void updateWindowMenu(QMdiSubWindow *window);
	void subWindowActivate(QString itemName);
	void determineWindowAction(QAction *action);
	void showDockWidget(QAction* aciton);
	void switchLanguage(QAction *action);
	void showAllDocks();
	void hideAllDocks();

protected:
	void closeEvent(QCloseEvent *cevent);

private:
	Ui::MainWindow ui;
	
	std::map<QString, mdiData> subWindowList;
	std::map<QString, QAction*> curWindowAction;
	QList<QMdiSubWindow*> windows;
	QMdiArea *mdiArea;
	AxisOperate *axisOpt;
	ImageProc *imgProc;
	Histogram *hist;
	AxisPropertySheet *propertySheet;
	MotionMode *mode;
	SelecteImage *selectImg;
	AxisMotionInfo *motionInfo;
	PlotWidget *plotWidget;
	DiyDockWidget *histDock;
	DiyDockWidget *imgProcDock;
	DiyDockWidget *axisStatDock;
	DiyDockWidget *axisSettings;
	DiyDockWidget *axisOptDock;
	DiyDockWidget *modeDock;
	DiyDockWidget *selectDock;
	DiyDockWidget *motionDock;
	DiyDockWidget *propertyDock;

	QLabel *xAxisLabel;
	QPushButton *plotButton;
	QPushButton *histEquilize;
	ThreadInfo *info;

	QString org;
	QString softwareName;
	QString windowName;
};

#endif //MAINWINDOW_H
