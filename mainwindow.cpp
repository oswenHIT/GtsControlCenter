#include "mainwindow.h"
#include <gts.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	mdiArea = new QMdiArea;
	imgProc = new ImageProc;
	hist = new Histogram;
	propertySheet = new AxisPropertySheet;
	info = new ThreadInfo;
	axisOpt = new AxisOperate;
	mode = new MotionMode(axisOpt, mdiArea, this);
	selectImg = new SelecteImage;
	plotWidget = new PlotWidget(this);
	motionInfo = new AxisMotionInfo;
	histDock = new DiyDockWidget();
	histDock->setWindowTitle(tr("Histogram Operations"));
	imgProcDock = new DiyDockWidget();
	imgProcDock->setWindowTitle(tr("Illuminating Console"));
	axisStatDock = new DiyDockWidget();
	axisStatDock->setWindowTitle(tr("Axis Stat Information"));
	axisSettings = new DiyDockWidget();
	axisSettings->setWindowTitle(tr("Axis Settings"));
	axisOptDock = new DiyDockWidget();
	axisOptDock->setWindowTitle(tr("Axis Operation"));
	modeDock = new DiyDockWidget();
	modeDock->setWindowTitle(tr("Motion Mode"));
	selectDock = new DiyDockWidget();
	selectDock->setWindowTitle(tr("Select Image"));
	motionDock = new DiyDockWidget();
	motionDock->setWindowTitle(tr("Axis Motion Information"));
	propertyDock = new DiyDockWidget();
	propertyDock->setWindowTitle(tr("Axis Property Sheet"));

	org = "HIT Software Inc.";
	softwareName = "Motion Control Panel";
	windowName = "Main Window";

	createMainWidget();
	createWhatsThis();
	createLanguageMenu();
	createViewMenu();
	buildSignalSlot();

	mdiArea->setViewMode(QMdiArea::TabbedView);
	mdiArea->setTabsClosable(true);
	mdiArea->setTabsMovable(true);
	mdiArea->setPalette(QPalette(QColor(240, 255, 255)));
	QMdiSubWindow *mdiWindow = mdiArea->addSubWindow(plotWidget);
	mdiWindow->setWindowTitle(tr("Plot Window"));
	mdiWindow->showMaximized();

	setCentralWidget(mdiArea);
	setWindowTitle(tr("Motion Control Center"));
	statusBar()->setSizeGripEnabled(true);
	statusBar()->setForegroundRole(QPalette::BrightText);
	statusBar()->setStyleSheet("background:rgb(0, 154, 215);color: rgb(248, 248, 255);");
	readSettings();  
}

MainWindow::~MainWindow()
{

}

void MainWindow::buildSignalSlot()
{
	connect(ui.actionHide_All, &QAction::triggered,
		this, &MainWindow::hideAllDocks);
	connect(ui.actionShow_All, &QAction::triggered,
		this, &MainWindow::showAllDocks);
	connect(ui.actionOpen_Picture, &QAction::triggered,
		this, &MainWindow::openPicture);
	connect(ui.actionAbout_App, &QAction::triggered,
		this, &MainWindow::aboutApp);
	connect(ui.actionAbout_QT, &QAction::triggered,
		qApp, &QApplication::aboutQt);
	connect(ui.actionAbout_GTS_400, &QAction::triggered,
		axisOpt, &AxisOperate::aboutGTS_400);
	connect(ui.actionPrevious_Window, &QAction::triggered,
		mdiArea, &QMdiArea::activatePreviousSubWindow);
	connect(ui.actionNext_Window, &QAction::triggered,
		mdiArea, &QMdiArea::activateNextSubWindow);
	connect(ui.actionClose_All, &QAction::triggered,
		mdiArea, &QMdiArea::closeAllSubWindows);
	connect(ui.menuWindow, &QMenu::triggered,
		this, &MainWindow::determineWindowAction);
	connect(ui.menuLanguage, &QMenu::triggered,
		this, &MainWindow::switchLanguage);
	connect(ui.menuView, &QMenu::triggered,
		this, &MainWindow::showDockWidget);
	connect(ui.actionQuit, &QAction::triggered,
		this, &MainWindow::close);
	connect(mdiArea, &QMdiArea::subWindowActivated,
		this, &MainWindow::updateWindowMenu);
	connect(selectImg, &SelecteImage::sendClickedItem,
		this, &MainWindow::subWindowActivate);

	connect(axisOpt, &AxisOperate::sendAxisStat,
		info, &ThreadInfo::processAxisStat);
	connect(axisOpt, &AxisOperate::gtsHasClosed,
		info, &ThreadInfo::resetAxisStat);
	connect(axisOpt, &AxisOperate::gtsHasClosed,
		mode, &MotionMode::gtsClosed);
	connect(info, &ThreadInfo::axisDisabled,
		axisOpt, &AxisOperate::disableAxisCheckStat);

	connect(axisOpt, &AxisOperate::sendAxisMotionInfo,
		motionInfo, &AxisMotionInfo::processAxisMotionInfo);
	connect(axisOpt, &AxisOperate::gtsHasClosed,
		motionInfo, &AxisMotionInfo::resetAxisMotionInfo);
	connect(axisOpt, &AxisOperate::showAxisMode,
		motionInfo, &AxisMotionInfo::showAxisMode);
	connect(axisOpt, &AxisOperate::hideAxisMode,
		motionInfo, &AxisMotionInfo::hideAxisMode);

	connect(axisOpt, &AxisOperate::sendModeData,
		mode, &MotionMode::receiveModeData);
	connect(axisOpt, &AxisOperate::gtsHasOpened,
		mode, &MotionMode::enableButton);
	connect(axisOpt, &AxisOperate::gtsHasClosed,
		mode, &MotionMode::disableButton);

	connect(mode, &MotionMode::stopPlotSignal,
		plotWidget, &PlotWidget::stopPlot);
	connect(mode, &MotionMode::startPlotSignal,
		plotWidget, &PlotWidget::startPlot);
	connect(axisOpt, &AxisOperate::sendModeData,
		plotWidget, &PlotWidget::receiveModeData);
}

void MainWindow::createMainWidget()
{
	selectDock->setWidget(selectImg);
	selectDock->setAllowedAreas(Qt::LeftDockWidgetArea
		| Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, selectDock);

	histDock->setWidget(hist);
	histDock->setAllowedAreas(Qt::LeftDockWidgetArea
		| Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, histDock);

	imgProcDock->setWidget(imgProc);
	imgProcDock->setAllowedAreas(Qt::LeftDockWidgetArea
		| Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, imgProcDock);

	axisStatDock->setWidget(info);
	axisStatDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, axisStatDock);

	motionDock->setWidget(motionInfo);
	motionDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, motionDock);

	QTableWidget *table2 = new QTableWidget;
	QDockWidget *rightBottomDock = new QDockWidget(tr("Axis Stat"));
	rightBottomDock->setWidget(table2);
	rightBottomDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, rightBottomDock);
	axisOpt->show();

	modeDock->setWidget(mode);
	modeDock->setAllowedAreas(Qt::LeftDockWidgetArea
		| Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, modeDock);

	propertyDock->setWidget(propertySheet);
	propertyDock->setAllowedAreas(Qt::LeftDockWidgetArea
		| Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, propertyDock);

	tabifyDockWidget(propertyDock, modeDock);
}

void MainWindow::writeSettings()
{
	QSettings settings(org, softwareName);

	settings.beginGroup(windowName);
	settings.setValue("Geometry", saveGeometry());
	settings.setValue("Stat", saveState());
	settings.setValue("Histogram Size", hist->size());
//	settings.setValue("Property Sheet Size", propertySheet->size());
	settings.endGroup();
}

void MainWindow::readSettings()
{
	QSettings settings(org, softwareName);

	settings.beginGroup(windowName);
	restoreGeometry(settings.value("Geometry").toByteArray());
	restoreGeometry(settings.value("Stat").toByteArray());
	restoreGeometry(settings.value("Histogram Size").toByteArray());
	restoreGeometry(settings.value("Property Sheet Size").toByteArray());
	settings.endGroup();
}

void MainWindow::updateWindowMenu(QMdiSubWindow *window)
{
	windows = mdiArea->subWindowList();
	std::map<QString, mdiData> windowName;

	windowMenuClear();
	for (int i = 0; i < windows.size(); i++)
	{
		QMdiSubWindow *tmp = windows.at(i);
		if (windows.at(i)->windowTitle() == QString("Plot Window"))
			continue;

		QAction *action = ui.menuWindow->addAction(windows.at(i)->windowTitle());
		curWindowAction.insert(std::make_pair(windows.at(i)->windowTitle(), action));
		windowName.insert(std::make_pair(windows.at(i)->windowTitle(), mdiData()));
		action->setCheckable(true);
		action->setChecked(tmp == mdiArea->activeSubWindow());
	}

	if (windowName.size() != subWindowList.size())
	{
		std::map<QString, mdiData>::const_iterator iter1;
		for (iter1 = windowName.begin(); iter1 != windowName.end(); ++iter1)
		{
			std::map<QString, mdiData>::const_iterator iter2;
			for (iter2 = subWindowList.begin(); iter2 != subWindowList.end(); ++iter2)
			{
				if (iter1->first == iter2->first)
				{
					subWindowList[iter1->first].flag = true;
				}
			}
		}

		iter1 = subWindowList.begin();
		while ( iter1 != subWindowList.end())
		{
			if (iter1->second.flag == false)
			{
				selectImg->deleteItem(iter1->first);
				delete iter1->second.scrollArea;
				delete iter1->second.window;
				subWindowList.erase(iter1++);
			}
			else
			{
				subWindowList[iter1->first].flag = false;
				iter1++;
			}
		}
	}
}

void MainWindow::subWindowActivate(QString itemName)
{
	std::map<QString, mdiData>::const_iterator iter = subWindowList.cbegin();

	while (iter != subWindowList.cend())
	{
		if (iter->first == itemName)
		{
			mdiArea->setActiveSubWindow(iter->second.window);
		}
		
		iter++;
	}
}

void MainWindow::showDockWidget(QAction* action)
{

}

void MainWindow::determineWindowAction(QAction *action)
{
	std::map<QString, QAction*>::const_iterator 
		iter1 = curWindowAction.cbegin();

	while (iter1 != curWindowAction.cend())
	{
		if (iter1->second == action)
		{
			std::map<QString, mdiData>::const_iterator iter2 
				= subWindowList.cbegin();
			while (iter2 != subWindowList.cend())
			{
				if (iter2->first == iter1->first)
				{
					mdiArea->setActiveSubWindow(iter2->second.window);
					return;
				}

				++iter2;
			}

			QMessageBox::information(this,
				tr("Warning"), tr("Sub-window doesn't exist!"));
			return;
		}

		++iter1;
	}
}

void MainWindow::windowMenuClear()
{
	std::map<QString, QAction*>::const_iterator iter
		= curWindowAction.cbegin();

	while (iter != curWindowAction.cend())
	{
		ui.menuWindow->removeAction(iter->second);
		delete iter->second;
		curWindowAction.erase(iter++);
	}
}

void MainWindow::aboutApp()
{
	QMessageBox::information(
		this, 
		tr("About Application"),
		tr("This is a software powered by Oswen Wang HITsz.\n"
		   "You can use it for educational intention.")
		);
}

void MainWindow::closeEvent(QCloseEvent *cevent)
{
	int ret;
	ret = QMessageBox::information(this, tr("Close"),
		tr("Do you want to close this window?"),
		QMessageBox::Yes | QMessageBox::Cancel);

	if (ret == QMessageBox::Yes)
	{
		writeSettings();
		QApplication::quit();
	}
	else
	{
		cevent->ignore();
	}
}

void MainWindow::openPicture()
{
	QString imgName;
	QList<QString>::Iterator namesIter;
	QList<QString> imgFullNames = QFileDialog::getOpenFileNames(
		this, 
		tr("Select one or more images to open"),
		"d:/Picture",
		tr("Images (*.png *.xpm *.jpg)"));
	
	int cnt = 1;
	QProgressDialog progress("Loading images...", "Abort open", 0, imgFullNames.size(), this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumDuration(2000);

	for (namesIter = imgFullNames.begin(); 
		 namesIter != imgFullNames.end(); 
		 ++namesIter, ++cnt)
	{
		if (!(*namesIter).isEmpty())
		{
			QFileInfo info = QFileInfo(*namesIter);
			imgName = info.fileName();

			std::map<QString, mdiData>::const_iterator iter = subWindowList.cbegin();
			while (iter != subWindowList.cend())
			{
				if (iter->first == imgName)
				{
					QMessageBox::information(this,
						tr("Warning"), tr("Same image has been opened!"));
					return;
				}
				++iter;
			}

			mdiData data;
			DiyScrollArea *newArea = new DiyScrollArea;
			newArea->loadImage(*namesIter);
			QMdiSubWindow *newWindow = mdiArea->addSubWindow(newArea);
			data.window = newWindow;
			data.scrollArea = newArea;
			subWindowList.insert(std::make_pair(imgName, data));
			selectImg->addListItem(imgName);
			newWindow->setWindowTitle(imgName);
			newWindow->show();
		}

		progress.setValue(cnt);
	}
}

void MainWindow::createViewMenu()
{
	QAction *action;

	action = selectDock->toggleViewAction();
	action->setText(tr("Select"));
	ui.menuView->addAction(action);

	action = histDock->toggleViewAction();
	action->setText(tr("Histogram"));
	ui.menuView->addAction(action);

	action = imgProcDock->toggleViewAction();
	action->setText(tr("DIP"));
	ui.menuView->addAction(action);

	action = modeDock->toggleViewAction();
	action->setText(tr("Mode"));
	ui.menuView->addAction(action);

	action = propertyDock->toggleViewAction();
	action->setText("Axis Property Sheet");
	ui.menuView->addAction(action);

	action = motionDock->toggleViewAction();
	action->setText(tr("Motion-Stat"));
	ui.menuView->addAction(action);

	action = axisStatDock->toggleViewAction();
	action->setText(tr("Axis-Stat"));
	ui.menuView->addAction(action);
} 


void MainWindow::switchLanguage(QAction *action)
{

}

void MainWindow::showAllDocks()
{
	selectDock->show();
	histDock->show();
	imgProcDock->show();
	modeDock->show();
	propertyDock->show();
	motionDock->show();
	axisStatDock->show();
}

void MainWindow::hideAllDocks()
{
	selectDock->hide();
	histDock->hide();
	imgProcDock->hide();
	modeDock->hide();
	propertyDock->hide();
	motionDock->hide();
	axisStatDock->hide();
}

void MainWindow::createWhatsThis()
{
	QAction *action = QWhatsThis::createAction();
	action->setText(tr("What's this"));
	action->setStatusTip(tr("Enter what's this mode"));
	ui.menuHelp->addAction(action);
}

void MainWindow::createLanguageMenu()
{
	/*QActionGroup *languageGroup = new QActionGroup(this);
	connect(languageGroup, &QActionGroup::triggered,
		this, &MainWindow::switchLanguage);

	QDir dir("F:/QtProject/GtsControlCenter/GtsControlCenter");
	QStringList fileNames 
		= dir.entryList(QStringList("gtscontrolcenter_*.qm"));

	for (int i = 0; i < fileNames.size(); i++)
	{
		QTranslator translator;
		translator.load(fileNames[i], dir.absolutePath());

		QString language = translator.translate("MainWindow", "English");
		QAction *action = new QAction(tr("%1 %2").arg(i + 1).arg(language), this);

		action->setCheckable(true);
		ui.menuLanguage->addAction(action);
		languageGroup->addAction(action);

		if (language == "English")
			action->setChecked(true);
	}
	qDebug() << fileNames.size();*/
}
