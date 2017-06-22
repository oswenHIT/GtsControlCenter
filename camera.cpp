
#include "camera.h"


Camera::Camera(QWidget *parent)
	: QMainWindow(parent), curIndex(0), curCount(0), isGrabMode(false),
	isDeleted(delStat::original), isInit(false), isPause(false), 
	timer(new QTimer(this)), process(new QProcess(this)),
	drivePath("C:/Program Files/The Imaging Source Europe GmbH/GigECam Driver and Tools/IPConfig.exe")
{
	ui.setupUi(this);

	centralMdi = new QMdiArea;
	centralMdi->addSubWindow(ui.imgLabel);
	setCentralWidget(centralMdi);

	ui.imgLabel->setGrabber(&mGrabber);
	listner = new CListener;
	ui.imgLabel->setListener(listner);

	pFilter.attach(new WRFilter);
	mGrabber.setDeviceFrameFilters(pFilter.get());

	ui.stopButton->setEnabled(false);
	ui.actionStop->setEnabled(false);
	ui.settingButton->setEnabled(false);
	ui.actionSettings->setEnabled(false);
	ui.snapButton->setEnabled(false);
	ui.actionSave->setEnabled(false);
	ui.pauseButton->setEnabled(false);
	ui.actionPause->setEnabled(false);
	ui.actionGrabMode->setEnabled(false);

	this->statusBar()->addPermanentWidget(ui.cameraModel);
	this->statusBar()->addPermanentWidget(ui.cameraResolution);
	this->statusBar()->addPermanentWidget(ui.cameraFPS);
	this->statusBar()->addPermanentWidget(ui.grabMode);
	this->statusBar()->addPermanentWidget(ui.timeLabel);
	this->menuBar()->setCornerWidget(ui.buttonWidget);

	createDockWidget();
	buildCommunication();

	initCamera();

	timer->start(1000);
}


Camera::~Camera()
{
	if (isDeleted == delStat::notyet)
	{
		for (auto i = 0; i < 10; i++)
			delete [] buffer[i];

		delete listner;
	}

	delete timer;
	delete process;
	delete mdiArea;
	delete axisOpt;
	delete mode;
	delete sheet;
}


void Camera::createDockWidget()
{
	mdiArea = new QMdiArea;
	axisOpt = new AxisOperate;
	mode = new MotionMode(axisOpt, mdiArea, this);
	sheet = new AxisPropertySheet;

	auto cameraDock = new DiyDockWidget;
	cameraDock->setWindowTitle("Camera Operation");
	cameraDock->setWidget(ui.cameraWidget);
	cameraDock->setAllowedAreas(Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, cameraDock);

	auto lightDock = new DiyDockWidget;
	lightDock->setWindowTitle("Light Console");
	lightDock->setWidget(ui.lightControl);
	lightDock->setAllowedAreas(Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, lightDock);

	auto statDock = new DiyDockWidget;
	statDock->setWindowTitle("Stat Window");
	statDock->setWidget(ui.settingWidget);
	statDock->setAllowedAreas(Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, statDock);

	auto modeDock = new DiyDockWidget;
	modeDock->setWindowTitle("Axis Mode");
	modeDock->setWidget(mode);
	modeDock->setAllowedAreas(Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, modeDock);

	auto sheetDock = new DiyDockWidget;
	sheetDock->setWindowTitle("Axis Property Sheet");
	sheetDock->setWidget(sheet);
	sheetDock->setAllowedAreas(
		Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, sheetDock);
}


void Camera::enableFullScreen()
{
	menuBar()->cornerWidget()->setVisible(true);
	this->showFullScreen();
}


void Camera::toNormalMode()
{
	menuBar()->cornerWidget()->setVisible(false);
	this->showNormal();
}


void Camera::minimizeApp()
{
	this->showMinimized();
}


void Camera::exitApp()
{
	this->close();
}


void Camera::setDrivePath()
{
	auto path = QFileDialog::getExistingDirectory(
		this, 
		tr("Open Directory"),
		"C:/Program Files",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	
	if ("" == path)
	{
		QMessageBox::warning(
			this, "Error", "Invalid directory!");

		return;
	}
	drivePath = path + "/IPConfig.exe";
}


void Camera::setGrabMode()
{
	isGrabMode = isGrabMode ? false : true;

	if (isGrabMode)
		ui.actionGrabMode->setIcon(
			QIcon("F:/QtProject/GtsControlCenter/GtsControlCenter/Resources/grab2.ico"));
	else
	{
		ui.actionGrabMode->setIcon(
			QIcon("F:/QtProject/GtsControlCenter/GtsControlCenter/Resources/grab1.ico"));
		
		QMouseEvent event0(
			QEvent::MouseButtonPress, 
			QPoint(640, 480), Qt::LeftButton, 
			Qt::LeftButton, Qt::NoModifier);
		QMouseEvent event1(
			QEvent::MouseMove,
			QPoint(320, 480), Qt::LeftButton,
			Qt::LeftButton, Qt::NoModifier);
		QApplication::sendEvent(this->ui.imgLabel, &event0);
		QApplication::sendEvent(this->ui.imgLabel, &event1);
	}
	
	ui.imgLabel->setGrabMode(isGrabMode);

	isGrabMode ?
		ui.grabMode->setText("Grab: On") :
		ui.grabMode->setText("Grab: Off");
}


void Camera::enableHistEQL(int stat)
{
	bool isEnable;

	pFilter->beginParamTransfer();

	isEnable = (stat == Qt::Checked ? true : false);
	pFilter->setParameter("Hist EQL", isEnable);

	pFilter->endParamTransfer();
}


void Camera::setFilterMode(int mode)
{
	int type;
	switch (mode)
	{
	case 0: type = wr::FilterType::None; break;
	case 1: type = wr::FilterType::Gauss; break;
	case 2: type = wr::FilterType::Homogeneous; break;
	case 3: type = wr::FilterType::Median; break;
	case 4: type = wr::FilterType::Bilatrial; break;
	default: break;
	}

	pFilter->beginParamTransfer();
	qDebug() << "Type:" << type;
	pFilter->setParameter(
		"Filter Mode", static_cast<long>(type));

	pFilter->endParamTransfer();
}


void Camera::initCamera()
{
	//Initialize the library.
	DShowLib::InitLibrary();

	auto isEmpty = addCameraItem();
	if (!isEmpty)
	{
		QMessageBox::warning(
			this,
			"Camera",
			"Please open the drive program to configure a camera!");
	}

	//Let the DirectShow window attach to the label to display the image.
	auto wID = ui.imgLabel->winId();
	auto id = (HWND)wID;
	if (!mGrabber.setHWND(id))
	{
		errorMsgBox();
	}
}


bool Camera::addCameraItem()
{
	//Test if there are cameras attach to the computer
	pDevList = mGrabber.getAvailableVideoCaptureDevices();
	if (pDevList->empty())
	{
		return false;
	}
	if (curCount != pDevList->size())
	{
		auto count = ui.selectBox->count();
		for (auto i = 0; i < count; i++)
		{
			ui.selectBox->removeItem(i);
		}

		for (auto i : *pDevList)
		{
			QString str(i.c_str());
			ui.selectBox->addItem(str);
		}
		curCount = pDevList->size();
	}
	
	return true;
}


void Camera::openDriveProg()
{
	while (!process->startDetached(
		drivePath,
		QStringList()))
	{
		auto button = 
			QMessageBox::question(
			this,
			"Error",
			"Can not open the program\nCheck the path please!");
		if (QMessageBox::Yes == button)
			setDrivePath();
		else
			return;
	}

	while (!addCameraItem());
}


void Camera::createSinkListener()
{
	//Add a listener to the grabber
	listner->setBufferSize(10);
	mGrabber.addListener(listner);

	if (!mGrabber.setOverlayBitmapPathPosition(ePP_DISPLAY))
		errorMsgBox();
	mGrabber.getOverlay(ePP_DISPLAY)->setColorMode(
		_DSHOWLIB_NAMESPACE::OverlayBitmap::eCOLOR);
	mGrabber.getOverlay(ePP_DISPLAY)->setEnable(true);
	if (!mGrabber.addListener(listner,
		GrabberListener::eFRAMEREADY |
		GrabberListener::eOVERLAYCALLBACK))
		errorMsgBox();

	//Create a FrameHandlerSink for grabber
	pSink = DShowLib::FrameHandlerSink::create(eY800, 1);
	pSink->setSnapMode(true);
	if (!mGrabber.setSinkType(pSink))
	{
		errorMsgBox();
	}
	if (!mGrabber.prepareLive(true))
	{
		errorMsgBox();
	}

	//Create buffer
	pSink->getOutputFrameType(fType);
	for (int i = 0; i < 10; ++i)
	{
		buffer[i] = new BYTE[fType.buffersize];
	}

	//Set the buffer for the pSink
	pCollection = MemBufferCollection::create(fType, 10, buffer);
	if (pCollection == 0 || !pSink->setMemBufferCollection(pCollection))
	{
		QMessageBox::warning(
			this,
			"Camera",
			pCollection->getLastError().c_str());

		return;
	}

	isDeleted = delStat::notyet;
}


void Camera::openCamera()
{
	if (mGrabber.getPauseLive())
	{
		mGrabber.setPauseLive(false);
		isPause = false;
		return;
	}

	if (!mGrabber.openDev(pDevList->at(curIndex)))
	{
		errorMsgBox();
		return;
	}
	if (!mGrabber.isDevValid())
	{
		mGrabber.closeDev();
	}
	else
	{
		if (mGrabber.isLive())
		{
			mGrabber.stopLive();
		}
	}

	if (!isInit)
	{
		isInit = true;
		createSinkListener();
	}
	else
	{
		if (!mGrabber.prepareLive(true))
		{
			errorMsgBox();
		}
	}

	if (mGrabber.startLive(false))
	{
		ui.deviceButton->setEnabled(false);
	}
	else
	{
		errorMsgBox();
		return;
	}

	ui.cameraModel->setText(
		ui.cameraModel->text().left(11).append(
			ui.selectBox->currentText()));

	auto fps = mGrabber.getCurrentActualFPS();
	ui.cameraFPS->setText(
		ui.cameraFPS->text().left(5).append(QString::number(fps)));

	auto xSize = mGrabber.getAcqSizeMaxX();
	auto ySize = mGrabber.getAcqSizeMaxY();
	auto str = QString::number(xSize)
		+ "*" + QString::number(ySize);
	ui.cameraResolution->setText(
		ui.cameraResolution->text().left(12).append(str));
	
	ui.liveButton->setEnabled(false);
	ui.selectBox->setEnabled(false);
	ui.stopButton->setEnabled(true);
	ui.actionStop->setEnabled(true);
	ui.pauseButton->setEnabled(true);
	ui.actionPause->setEnabled(true);
	ui.settingButton->setEnabled(true);
	ui.actionSettings->setEnabled(true);
	ui.snapButton->setEnabled(true);
	ui.actionSave->setEnabled(true);
	ui.actionGrabMode->setEnabled(true);
}


void Camera::closeCamera()
{
	if (!mGrabber.stopLive())
		errorMsgBox();

	ui.cameraModel->setText("CurCamera: ---");
	ui.cameraFPS->setText("FPS: ---");
	ui.cameraResolution->setText("Resolution: ---");

	ui.liveButton->setEnabled(true);
	ui.selectBox->setEnabled(true);
	ui.stopButton->setEnabled(false);
	ui.actionStop->setEnabled(false);
	ui.pauseButton->setEnabled(false);
	ui.actionPause->setEnabled(false);
	ui.settingButton->setEnabled(false);
	ui.actionSettings->setEnabled(false);
	ui.deviceButton->setEnabled(true);
	ui.actionCamrera->setEnabled(true);
	ui.snapButton->setEnabled(false);
	ui.actionSave->setEnabled(false);
	ui.actionGrabMode->setEnabled(false);
}


void Camera::pauseCamera()
{
	auto isOn = isPause ? false : true;
	auto buttonName = isPause ? "Pause" : "Start";
	
	if (!mGrabber.setPauseLive(isOn))
		errorMsgBox();
	ui.pauseButton->setText(QString(buttonName));

	mGrabber.getOverlay(ePP_DISPLAY)->setEnable(!isOn);

	isPause = isPause ? false : true;
}


void Camera::openSetting()
{
	if (!mGrabber.showVCDPropertyPage())
		errorMsgBox();
}


void Camera::openDevice()
{
	if (!mGrabber.isLive())
	{
		if (!mGrabber.showDevicePage())
			errorMsgBox();
	}
}


void Camera::snapImage()
{
	static int count = 0;

	if (!mGrabber.getPauseLive())
	{
		auto err = pSink->snapImages(10);
		if (err.isError())
			QMessageBox::warning(this, "Snap error", err.c_str());

		auto tmpBuffer = pCollection->operator[](0);
		if (!tmpBuffer->isLocked())
		{
			tmpBuffer->lock();

			auto fName = std::string("d:/Test") 
				+ std::to_string(count) 
				+ ".jpg";
			err = saveToFileJPEG(*tmpBuffer, fName, 90);
			if (err.isError())
				QMessageBox::warning(this, "Snap error", err.c_str());

			//auto img = new cv::Mat(960, 1280, CV_8U);
			//img->data = buffer[0];
			//bool ret = false;
			
			//ret = cv::imwrite(fName, *img);
			//delete img;
			count++;

			tmpBuffer->unlock();
		}	
	}
	else
	{
		QMessageBox::warning(
			this,
			"Camera",
			"Under pause mode you can't snap the image!");
	}
}


void Camera::changeCurCamera(int index)
{
	curIndex = index;
}


void Camera::imageFlipH(bool checked)
{
	static bool isFlipH = true;

	if (mGrabber.isLive())
	{
		if (!mGrabber.stopLive())
			errorMsgBox();
		if (!mGrabber.setFlipH(isFlipH))
			errorMsgBox();
		if (!mGrabber.startLive(false))
			errorMsgBox();
	}

	isFlipH = isFlipH ? false : true;
}


void Camera::imageFlipV(bool checked)
{
	static bool isFlipV = true;

	if (mGrabber.isLive())
	{
		if (!mGrabber.stopLive())
			errorMsgBox();
		if (!mGrabber.setFlipV(isFlipV))
			errorMsgBox();
		if (!mGrabber.startLive(false))
			errorMsgBox();
	}

	isFlipV = isFlipV ? false : true;
}


void Camera::timerSlots()
{
	QTime qtimeObj = QTime::currentTime();
	QString strTime = qtimeObj.toString("hh:mm ap");
	strTime.prepend("  Time is: ");
	QDate qdateObj = QDate::currentDate();
	QString strDate = qdateObj.toString("dddd, MMMM d, yyyy");   
	strDate.prepend(" Today is: ");

	strDate.append(strTime);
	ui.timeLabel->setText(strDate);

	if (mGrabber.isLive())
	{
		if (mGrabber.getPauseLive())
			statusBar()->showMessage("Camera on pause mode...");
		else
			statusBar()->showMessage("Camera running...");
	}
	else
	{
		statusBar()->showMessage("Open an available camera.");
	}
}


void Camera::aboutQT()
{
	QMessageBox::aboutQt(this, "Powered by W.R. HIT");
}


void Camera::closeEvent(QCloseEvent * ev)
{
	auto quitSelect = QMessageBox::question(
		this,
		"Quit dialog",
		"Do you want to quit?");
	if (quitSelect == QMessageBox::Yes)
	{
		if (mGrabber.isDevOpen())
		{
			if (mGrabber.isLive())
			{
				if (!mGrabber.stopLive())
					errorMsgBox();
			}
			if (!mGrabber.closeDev())
			{
				errorMsgBox();
			}

			if (isDeleted == delStat::notyet)
			{
				for (auto i = 0; i < 10; i++)
					delete [] buffer[i];

				mGrabber.removeListener(listner);
				delete listner;

				isDeleted = delStat::deleted;
			}
		}

		ui.lightControl->closeLight();
	
		ev->accept();
	}
	else
	{
		ev->ignore();
	}
}


void Camera::keyPressEvent(QKeyEvent * ev)
{
	if (ev->key() == Qt::Key_G)
	{
		this->setGrabMode();
	}
	else if (ev->key() == Qt::Key_P)
	{
		if (mGrabber.isLive())
		{
			this->pauseCamera();
		}
	}
	else if (ev->key() == Qt::Key_F2)
	{
		this->toNormalMode();
		menuBar()->cornerWidget()->setVisible(false);
	}
	else if (ev->key() == Qt::Key_F1)
	{
		this->enableFullScreen()   ;
		menuBar()->cornerWidget()->setVisible(true);
	}
	else if (ev->key() == Qt::Key_F3)
	{
		this->showMinimized();
	}
	else if (ev->key() == Qt::Key_Escape)
	{
		this->exitApp();
	}
}


void Camera::errorMsgBox()
{
	statusBar()->showMessage(mGrabber.getLastError().c_str());

	QMessageBox::warning(
		this,
		"Camera error",
		mGrabber.getLastError().c_str());
}


void Camera::buildCommunication()
{
	connect(ui.liveButton, &QPushButton::clicked,
		[this] { this->openCamera(); });
	connect(ui.stopButton, &QPushButton::clicked,
		[this] { this->closeCamera(); });
	connect(ui.settingButton, &QPushButton::clicked,
		[this] { this->openSetting(); });
	connect(ui.deviceButton, &QPushButton::clicked,
		[this] { this->openDevice(); });
	connect(ui.selectBox, 
		static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		[this] (int index) 
		{ this->changeCurCamera(index); });
	connect(ui.snapButton, &QPushButton::clicked,
		[this] { this->snapImage(); });
	connect(ui.pauseButton, &QPushButton::clicked,
		[this] { this->pauseCamera(); });
	connect(ui.actionHorizontalFlip, &QAction::triggered,
		[this] (bool checkde) 
		{ this->imageFlipH(checkde); });
	connect(ui.actionVertivalFlip, &QAction::triggered,
		[this] (bool checked) 
		{ this->imageFlipV(checked); });
	connect(ui.actionBegin, &QAction::triggered,
		[this] { this->openCamera(); });
	connect(ui.actionPause, &QAction::triggered,
		[this] { this->pauseCamera(); });
	connect(ui.actionStop, &QAction::triggered,
		[this] { this->closeCamera(); });
	connect(ui.actionSettings, &QAction::triggered,
		[this] { this->openSetting(); });
	connect(ui.actionSave, &QAction::triggered,
		[this] { this->snapImage(); });
	connect(ui.actionCamrera, &QAction::triggered,
		[this] { this->openDevice(); });
	connect(ui.actionFullScreen, &QAction::triggered,
		[this] { this->enableFullScreen(); });
	connect(ui.actionExitFullS, &QAction::triggered,
		[this] { this->toNormalMode(); });
	connect(ui.actionExit, &QAction::triggered,
		[this] { this->exitApp(); });
	connect(ui.actionDrive, &QAction::triggered,
		[this] { this->openDriveProg(); });
	connect(ui.exitTool, &QToolButton::clicked,
		[this] { this->exitApp(); });
	connect(ui.fullTool, &QToolButton::clicked,
		[this] { this->enableFullScreen(); });
	connect(ui.quitTool, &QToolButton::clicked,
		[this] { this->toNormalMode(); });
	connect(timer, &QTimer::timeout,
		[this] { this->timerSlots(); });
	connect(ui.actionAbout_Qt, &QAction::triggered,
		[this] { this->aboutQT(); });
	connect(ui.actionPath, &QAction::triggered,
		[this] { this->setDrivePath(); });
	connect(ui.actionMinimize, &QAction::triggered,
		[this] { this->minimizeApp(); });
	connect(ui.minimizeTool, &QToolButton::clicked,
		[this] { this->minimizeApp(); });
	connect(ui.actionGrabMode, &QAction::triggered,
		[this] { this->setGrabMode(); });
	connect(ui.filterComboBox,
		static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		[this] (int index) 
		{ this->setFilterMode(index); });
	connect(ui.histCheckBox, &QCheckBox::stateChanged,
		[this] (int stat) 
		{ this->enableHistEQL(stat); });
}
