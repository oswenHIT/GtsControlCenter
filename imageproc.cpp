#include "imageproc.h"

ImageProc::ImageProc(QWidget *parent)
	: QWidget(parent), curChannel(0), curIntensity(0), flag(true)
{
	ui.setupUi(this);

	ui.connectButton->setEnabled(false);
	ui.openButton->setEnabled(false);
	ui.closeButton->setEnabled(false);

	getPorts();
	buildCommunication();
}

ImageProc::~ImageProc()
{
	;
}

void ImageProc::selectComPort(const QString &text)
{
	auto tmp = text;
	tmp.remove(0, 3);
	auto index = tmp.indexOf('(');
	tmp.remove(index, tmp.size() - 1);
	auto portNum = tmp.toInt();
	SetControllerComPort(portNum);
}


void ImageProc::activateButton()
{
	ui.connectButton->setEnabled(true);
}

void ImageProc::connectComPort()
{
	auto ret = ControllerInitialize();
	if (ret != 1)
	{
		QMessageBox::warning(
			this,
			tr("Light Source"),
			tr("Can not initialize Light Source!"));
	}
	else
	{
		if (flag)
		{
			getFirmVersion();
			flag = false;
		}
	}
}

void ImageProc::setCurChannel(int i)
{
	curChannel = i;
}

void ImageProc::setCurIntensity(int i)
{
	curIntensity = i;
}

void ImageProc::setIntensity()
{
	auto ret = SetControllerIntensity(curChannel, curIntensity);
	if (ret != 1)
	{
		QMessageBox::warning(
			this,
			tr("Light Source"),
			tr("Can not set light intensity!"));
	}
	else
		ui.openButton->setEnabled(true);
}

void ImageProc::getIntensity()
{
	auto ret = GetControllerIntensity(curChannel, &curIntensity);
	if (ret != 1)
	{
		QMessageBox::warning(
			this,
			tr("Light Source"),
			tr("Can not get Light intensity!"));
	}
	else
	{
		ui.intensitySpinBox->setValue(curIntensity);
	}
}

void ImageProc::openLight() const
{
	auto ret = EnableControllerOutput();
	if (ret != 1)
	{
		QMessageBox::warning(
			const_cast<ImageProc*>(this),
			tr("Light Source"),
			tr("Can not close light"));
	}
	else
		ui.closeButton->setEnabled(true);
}

void ImageProc::closeLight() const
{
	auto ret = DisableControllerOutput();
	if (ret != 1)
	{
		QMessageBox::warning(
			const_cast<ImageProc*>(this),
			tr("Light Source"),
			tr("Can not close Light Source!"));
	}
}

void ImageProc::getFirmVersion()
{
/*	using PVar = unsigned char*;
	PVar pvar1, pvar2, pvar3, pvar4;
	auto ret = GetControllerFirmwareVersion(pvar1, pvar2, pvar3, pvar4);
	if (ret != 1)
	{
		QMessageBox::warning(
			this,
			tr("Light Source"),
			tr("Can not get firm version!"));
	}
	else
	{
	/*	QString tmpStr;
		tmpStr = *pvar1;
		tmpStr += '.';
		tmpStr += *pvar2;
		tmpStr += '.';
		tmpStr += *pvar3;
		tmpStr += '.';
		tmpStr += *pvar4;
		ui.versionLabel->setText(tmpStr);
		qDebug() << *pvar1;
	}*/
	ui.versionLabel->setText(QString("1.2.3.4"));
}

void ImageProc::getPorts()
{
	auto port = QSerialPortInfo::availablePorts();

	QString tmpStr;
	for (auto i : port)
	{
		if (!i.isBusy())
		{
			tmpStr = 
				i.portName() + 
				'(' + 
				i.manufacturer() + 
				')';
			ui.portSelect->addItem(tmpStr);
		}
	}
}

void ImageProc::recheckPorts()
{
	;
}

void ImageProc::buildCommunication()
{
	connect(ui.portSelect,
		static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),
		[this](const QString& text) { this->selectComPort(text); });
	connect(ui.portSelect,
		static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
		[this](int index) { this->activateButton(); });
	connect(ui.recheckButton, &QPushButton::clicked,
		[this] { this->recheckPorts(); });
	connect(ui.connectButton, &QPushButton::clicked,
		[this] { this->connectComPort(); });
	connect(ui.channelSpinBox,
		static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		[this](int value) { this->setCurChannel(value); });
	connect(ui.intensitySpinBox, 
		static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		[this](int value) { this->setCurIntensity(value); });
	connect(ui.setButton, &QPushButton::clicked,
		[this] { this->setIntensity(); });
	connect(ui.getButton, &QPushButton::clicked,
		[this] { this->getIntensity(); });
	connect(ui.openButton, &QPushButton::clicked,
		[this] { this->openLight(); });
	connect(ui.closeButton, &QPushButton::clicked,
		[this] { this->closeLight(); });
}
