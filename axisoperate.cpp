#include "axisoperate.h"

AxisOperate::AxisOperate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	curAxisNum = 1;
	gtsOpenFlag = false;
	axisStatThread = new AxisStatMonitor(this);
	info = new MotionInfo;

	correlateAxis[1] = ui.axisOneCheckBox;
	correlateAxis[2] = ui.axisTwoCheckBox;
	correlateAxis[3] = ui.axisThreeCheckBox;
	correlateAxis[4] = ui.axisFourCheckBox;
	for (int i = 1; i <= 4; ++i)
	{
		axisSelected[i] = false;
	}

	buildSignalSlot();
	
	ui.gtsStopButton->setEnabled(false);
	ui.resetButton->setEnabled(false);
	ui.axisOptGroupBox->setEnabled(false);
}

AxisOperate::~AxisOperate()
{

}

void AxisOperate::buildSignalSlot()
{
	connect(ui.openButton, &QPushButton::clicked,
		this, &AxisOperate::gtsOpen);
	connect(ui.gtsStopButton, &QPushButton::clicked,
		this, &AxisOperate::gtsStop);
	connect(ui.statClearButton, &QPushButton::clicked,
		this, &AxisOperate::statClear);
	connect(ui.servoEnableButton, &QPushButton::clicked,
		this, &AxisOperate::servoEnable);
	connect(ui.servoDisableButton, &QPushButton::clicked,
		this, &AxisOperate::axisStop);
	connect(ui.resetButton, &QPushButton::clicked,
		this, &AxisOperate::gtsReset);
	connect(ui.posClearButton, &QPushButton::clicked,
		this, &AxisOperate::posClear);

	connect(ui.axisSelectSlider, &DiyCircularSlider::valueChanged,
		this, &AxisOperate::changeCurAxisNum);
	connect(ui.axisSelectSlider, &DiyCircularSlider::valueChanged,
		this, &AxisOperate::createModeData);

	connect(axisStatThread, &AxisStatMonitor::sendAxisStat,
		this, &AxisOperate::processAxisStat);
	connect(axisStatThread, &AxisStatMonitor::sendError,
		this, &AxisOperate::gtsCommandHandler);
	connect(axisStatThread, &AxisStatMonitor::sendAxisMotionInfo,
		this, &AxisOperate::processAxisMotionInfo);

	connect(ui.axisOneCheckBox, &QCheckBox::toggled,
		this, &AxisOperate::axisOneCheck);
	connect(ui.axisTwoCheckBox, &QCheckBox::toggled,
		this, &AxisOperate::axisTwoCheck);
	connect(ui.axisThreeCheckBox, &QCheckBox::toggled,
		this, &AxisOperate::axisThreeCheck);
	connect(ui.axisFourCheckBox, &QCheckBox::toggled,
		this, &AxisOperate::axisFourCheck);
}

void AxisOperate::gtsOpen()
{
	int flag = 0;

	if (!gtsOpenFlag)
	{
		flag = GT_Open();
		if (flag)
		{
			gtsCommandHandler("GT_Open", 0, flag);
		}
		else
		{
			initGts();
			gtsOpenFlag = true;
			emit gtsHasOpened();
			
			axisStatThread->start();

			ui.openButton->setStyleSheet("background-color: rgb(255, 0, 0);");
			ui.gtsStopButton->setEnabled(true);
			ui.resetButton->setEnabled(true);
			ui.axisOptGroupBox->setEnabled(true);

			emit ui.axisSelectSlider->valueChanged(0);
		}
	}
}

void AxisOperate::gtsStop()
{
	int flag = 0;
	
	if (gtsOpenFlag)
	{
		flag = GT_Stop(0x05, 0);
		gtsCommandHandler("GT_Stop", 0, flag);
		flag = GT_Close();
		if (flag)
		{
			gtsCommandHandler("GT_Close", 0, flag);
			return;
		}

		ui.openButton->setStyleSheet("background-color: rgb(0, 255, 255);");
		gtsOpenFlag = false;

		if (axisStatThread->isRunning())
		{
			axisStatThread->stop();
			axisStatThread->wait();

			ui.gtsStopButton->setEnabled(false);
			ui.resetButton->setEnabled(false);
			ui.axisOptGroupBox->setEnabled(false);

			for (int i = 0; i <= 4; ++i)
			{
				if (axisSelected[i])
				{
					flag = GT_AxisOff(i);
					gtsCommandHandler("GT_AxisOff", 0, flag);
				}
			}
		}

		emit gtsHasClosed();
	}
}

void AxisOperate::gtsReset()
{
	initGts();
}

void AxisOperate::posClear()
{
	int axis = 0, flag;
	for (int i = 0; i < axisData.correlateAxis.size(); ++i)
	{
		axis = axisData.correlateAxis[i];
		flag = GT_ZeroPos(axis);
		gtsCommandHandler("GT_ZeroPos", axis, flag);
	}
}

void AxisOperate::processAxisStat(long* stat)
{

	for (int i = 0; i < 4; i ++ )
	{
		axisStat[i] = stat[i];
	}
	
	emit sendAxisStat(axisStat);
}

void AxisOperate::processAxisMotionInfo(MotionInfo *motionInfo)
{
	info = motionInfo;
	emit sendAxisMotionInfo(info);
}

void AxisOperate::initGts()
{
	int flag = 0;
	char* version;

	flag = GT_Reset();
	gtsCommandHandler("GT_Reset", 0, flag);
	flag = GT_LoadConfig("GTS-400.cfg");
	gtsCommandHandler("GT_LoadConfig", 0, flag);
	flag = GT_GetVersion(&version);
	gtsCommandHandler("GT_GetVersion", 0, flag);
	gtsVersion = version;
	flag = GT_ClrSts(1, 4);
	gtsCommandHandler("GT_ClrSts", 0, flag);
}

void AxisOperate::statClear()
{
	int flag = 0;
	flag = GT_ClrSts(1, 4);
	gtsCommandHandler("GT_ClrSts", 0, flag);

	for (int i = 1; i <= 4; ++i)
	{
		if (axisSelected[i])
		{
			flag = GT_ZeroPos(i);
			gtsCommandHandler("GT_ZeroPos", 0, flag);
		}
	}
}

void AxisOperate::servoEnable()
{
	int flag = 0;

	if (gtsOpenFlag)
	{

		for (int i = 0; i <= 4; ++i)
		{
			if (axisSelected[i])
			{
				flag = GT_AxisOn(i);
				gtsCommandHandler("GT_AxisOn", 0, flag);
			}
		}
	}
}

void AxisOperate::axisStop()
{
	int flag = 0;

	if (gtsOpenFlag)
	{
		
		for (int i = 0; i <= 4; ++i)
		{
			if (axisSelected[i])
			{
				flag = GT_AxisOff(i);
				gtsCommandHandler("GT_AxisOff", i, flag);
			}
		}
	}
	else
	{
		flag = GT_AxisOff(curAxisNum);
		gtsCommandHandler("GT_AxisOff", curAxisNum, flag);
	}
}

void AxisOperate::changeCurAxisNum(int axis)
{
	if (axis != 0)
		curAxisNum = axis;
}

void AxisOperate::axisOneCheck(bool checked)
{
	if (checked)
	{
		axisSelected[1] = true;
	}
	else
	{
		axisSelected[1] = false;
	}
	
	emit ui.axisSelectSlider->valueChanged(0);
}

void AxisOperate::axisTwoCheck(bool checked)
{
	if (checked)
	{
		axisSelected[2] = true;
	}
	else
	{
		axisSelected[2] = false;
	}
	
	emit ui.axisSelectSlider->valueChanged(0);
}

void AxisOperate::axisThreeCheck(bool checked)
{
	if (checked)
	{
		axisSelected[3] = true;
	}
	else
	{
		axisSelected[3] = false;
	}

	emit ui.axisSelectSlider->valueChanged(0);
}

void AxisOperate::axisFourCheck(bool checked)
{
	if (checked)
	{
		axisSelected[4] = true;
	}
	else
	{
		axisSelected[4] = false;
	}

	emit ui.axisSelectSlider->valueChanged(0);
}

void AxisOperate::disableAxisCheckStat(int axis)
{
	;
}

void AxisOperate::createModeData(int val)
{
	axisData.curAxisNum = curAxisNum;
	axisData.correlateAxis.clear();
	for (int i = 1; i <= 4; ++i)
	{
		if (axisSelected[i])
		{
			axisData.correlateAxis.push_back(i);
		}
	}

	emit sendModeData(&axisData);
}

void AxisOperate::gtsCommandHandler(QString cmdName, int axis, int flag)
{
	if (flag)
	{
		switch (flag)
		{
		case 1:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 \n"
				   "%2: Execution of the instruction error").arg(axis).arg(cmdName));
			break;

		case 2:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 \n"
				   "%2: Gts Controller does not support this feature").arg(axis).arg(cmdName));
			break;

		case 7:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 \n"
				   "%2: Instruction parameter error").arg(axis).arg(cmdName));
			break;

		case -1:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 \n"
				   "%2: Host and motion controller communication failed").arg(axis).arg(cmdName));
			break;

		case -6:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 \n"
				   "%2: Failed to open the controller").arg(axis).arg(cmdName));
			break;

		case -7:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 \n"
				   "%2: Motion controller does not respond").arg(axis).arg(cmdName));
			break;

		default:
			break;
		}
	}
}

void AxisOperate::aboutGTS_400()
{
	QString dspVersion;
	QString dspDate;
	QString fpgaVersion;
	QString fpgaDate;

	dspVersion = gtsVersion.left(3);
	dspVersion = dspVersion.left(1) 
			   + "." 
			   + dspVersion.mid(1);
	dspDate = gtsVersion.mid(3, 6);
	dspDate = "20" 
			+ dspDate.left(2) 
			+ "." 
			+ dspDate.mid(2, 2) 
			+ "." 
			+ dspDate.mid(4, 2);

	fpgaVersion = gtsVersion.mid(9, 3);
	fpgaVersion = fpgaVersion.left(1) 
				+ "." 
				+ fpgaVersion.mid(1);
	fpgaDate = gtsVersion.mid(12, 6);
	fpgaDate = "20" 
		     + fpgaDate.left(2) 
			 + "." 
			 + fpgaDate.mid(2, 2) 
			 + "." 
			 + fpgaDate.mid(4, 2);

	QMessageBox::information(
		this,
		tr("About GTS-400"),
		tr("Googool GTS-400 Motion Controller\n"
		"DSP  Version: %1\n"
		"DSP  Release Date: %2\n"
		"FPGA Version: %3\n"
		"FPGA Release Date: %4").arg(dspVersion).arg(dspDate).arg(fpgaVersion).arg(fpgaDate)
		);
}