#include "motionmode.h"

MotionMode::MotionMode(AxisOperate *operate, QMdiArea *mdi, QMainWindow *window)
	: QWidget(0)
{
	pidSetting = new PIDSetting;
	jogOpt = new JogOpt;
	ppOpt = new PointPosOpt;
	motionControl = new MotionControl(this);

	mainWindow = window;
	mdiArea = mdi;
	axisOperate = operate;
	isGtsOpen = false;
	jogDirec = false;
	foreOrBack = false;
	pidParameter[1] = &pidAxis1;
	pidParameter[2] = &pidAxis2;
	pidParameter[3] = &pidAxis3;
	pidParameter[4] = &pidAxis4;
	jogModeVariable[1] = &axis1;
	jogModeVariable[2] = &axis2;
	jogModeVariable[3] = &axis3;
	jogModeVariable[4] = &axis4;
	ppModeVariable[1] = &ppAxis1;
	ppModeVariable[2] = &ppAxis2;
	ppModeVariable[3] = &ppAxis3;
	ppModeVariable[4] = &ppAxis4;

	ui.setupUi(this);
	initTable();
	setTableWidget();
	createPIDCoordinate();
	createJogCoordinate();
	createPPCoordinate();
	buildSignalSlot();

	jogItem = ui.tableWidget->item(20, 0);
	ppItem = ui.tableWidget->item(24, 0);

	jogOpt->ui.startButton->setEnabled(false);
	jogOpt->ui.stopButton->setEnabled(false);
	ppOpt->ui.startButton->setEnabled(false);
	ppOpt->ui.stopButton->setEnabled(false);
}

MotionMode::~MotionMode()
{

}

void MotionMode::buildSignalSlot()
{
	connect(motionControl, &MotionControl::gtsCommandHandler,
		this, &MotionMode::gtsCommandHandler);
	connect(ui.tableWidget, &QTableWidget::cellClicked,
		this, &MotionMode::enableMotionMode);
	connect(motionControl, &MotionControl::showAxisMode,
		axisOperate, &AxisOperate::showAxisMode);
	connect(this, &MotionMode::hideAxisMode,
		axisOperate, &AxisOperate::hideAxisMode);

	connect(this, &MotionMode::trapStart, 
		motionControl, &MotionControl::motionStart);
	connect(this, &MotionMode::infoStart,
		motionControl, &MotionControl::infoBegin);
	connect(this, &MotionMode::infoEnd,
		motionControl, &MotionControl::infoEnd);
	connect(this, &MotionMode::sendJogParameter,
		motionControl, &MotionControl::setJogParameter);
	connect(this, &MotionMode::sendPPParameter,
		motionControl, &MotionControl::setPPParameter);
	connect(this, &MotionMode::jogModeActivated,
		motionControl, &MotionControl::activateJogMode);
	connect(this, &MotionMode::jogModeClosed,
		motionControl, &MotionControl::disableJogMode);
	connect(this, &MotionMode::ppModeActivated,
		motionControl, &MotionControl::activatePPMode);
	connect(this, &MotionMode::ppModeClosed,
		motionControl, &MotionControl::disablePPMode);
	connect(this, &MotionMode::jogMotionDirec,
		motionControl, &MotionControl::jogMove);
	connect(motionControl, &MotionControl::finished,
		this, &MotionMode::threadFinished);

	connect(jogOpt->ui.loadButton, &QPushButton::clicked,
		this, &MotionMode::loadJogSetting);
	connect(jogOpt->ui.startButton, &QPushButton::clicked,
		this, &MotionMode::jogStartButton);
	connect(jogOpt->ui.startButton, &QPushButton::clicked,
		this, &MotionMode::startPlot);
	connect(jogOpt->ui.stopButton, &QPushButton::clicked,
		motionControl, &MotionControl::stop);
	connect(jogOpt->ui.stopButton, &QPushButton::clicked,
		this, &MotionMode::jogStopButton);
	connect(jogOpt->ui.stopButton, &QPushButton::clicked,
		this, &MotionMode::stopPlot);
	connect(motionControl, &MotionControl::startJogMotion,
		this, &MotionMode::startMotion);

	connect(ppOpt->ui.loadSettingButton, &QPushButton::clicked,
		this, &MotionMode::loadPPSetting);
	connect(ppOpt->ui.startButton, &QPushButton::clicked,
		this, &MotionMode::startMotion);
	connect(ppOpt->ui.startButton, &QPushButton::clicked,
		this, &MotionMode::startPlot);
	connect(ppOpt->ui.stopButton, &QPushButton::clicked,
		motionControl, &MotionControl::stop);
	connect(ppOpt->ui.stopButton, &QPushButton::clicked,
		this, &MotionMode::stopPlot);
}

void MotionMode::stopPlot()
{
	emit stopPlotSignal();
}

void MotionMode::startPlot()
{
	emit startPlotSignal();
}

void MotionMode::startMotion()
{
	emit trapStart();
	motionControl->start();
}

void MotionMode::threadFinished()
{
	jogOpt->ui.startButton->setEnabled(false);
	jogOpt->ui.stopButton->setEnabled(false);
	ppOpt->ui.startButton->setEnabled(false);
	ppOpt->ui.stopButton->setEnabled(false);
}

void MotionMode::setTableWidget()
{
	ui.tableWidget->setSpan(0, 0, 11, 3);
	ui.tableWidget->setCellWidget(0, 0, axisOperate);

	ui.tableWidget->setSpan(13, 2, 7, 1);
	ui.tableWidget->setCellWidget(13, 2, pidSetting);
	pidSetting->setEnabled(false);

	ui.tableWidget->setSpan(21, 2, 3, 1);
	ui.tableWidget->setCellWidget(21, 2, jogOpt);
	jogOpt->setEnabled(false);

	ui.tableWidget->setSpan(25, 2, 7, 1);
	ui.tableWidget->setCellWidget(25, 2, ppOpt);
	ppOpt->setEnabled(false);
}

void MotionMode::initTable()
{
	QTableWidgetItem *item;

	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		if (i == 11 || i == 12 || i == 20 || i == 24)
		{
			for (int j = 0; j < ui.tableWidget->columnCount(); ++j)
			{
				item = ui.tableWidget->item(i, j);
				if (item != 0)
				{
					item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				}
			}
		}
		else
		{
			item = ui.tableWidget->item(i, 0);
			if (item != 0)
			{
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			}
		}
	}
}

void MotionMode::createPIDCoordinate()
{
	QTableWidgetItem *item;
	QString ele;

	for (int i = 13; i < 20; ++i)
	{
		ele = coorToString(i, 2);
		item = ui.tableWidget->item(i, 1);
		if (item != 0)
		{
			pidParCoordinate[ele] = item;
			item->setText("---");
		}
	}
}

void MotionMode::createJogCoordinate()
{
	QTableWidgetItem *item;
	QString ele;

	for (int i = 21; i < 24; ++i)
	{
		ele = coorToString(i, 2);
		item = ui.tableWidget->item(i, 1);
		if (item != 0)
		{
			jogParCoordinate[ele] = item;
			item->setText("---");
		}
	}
}

void MotionMode::createPPCoordinate()
{
	QTableWidgetItem *item;
	QString ele;

	for (int i = 25; i < 32; ++i)
	{
		ele = coorToString(i, 2);
		item = ui.tableWidget->item(i, 1);
		if (item != 0)
		{
			ppParCoordinate[ele] = item;
			item->setText("---");
		}
	}
}

void MotionMode::receiveModeData(toMotionModeData *data)
{
	QString axisNum, text1, text2;
	QVector<int>::ConstIterator iter = data->correlateAxis.cbegin();
	motionData = data;

	axisNum.setNum(data->curAxisNum);
	text1 = "[ " + axisNum + " ]";

	text2 = "[ ";
	while (iter != data->correlateAxis.cend())
	{
		axisNum.setNum(*iter);
		axisNum = axisNum + ",";
		text2 = text2 + axisNum;
		++iter;
	}
	text2 = text2.left(text2.size() - 1);
	text2 = text2 + " ]";
	if (text2 == "[ ]")
		text2 = "---";

	ui.tableWidget->item(11, 1)->setText(text1);
	ui.tableWidget->item(11, 2)->setText(text2);

	if (isGtsOpen)
	{
		displayJogModeInfo();
		getJogData();
		displayPPModeInfo();
		getPPData();
	}
}

void MotionMode::displayJogModeInfo()
{
	QString ele, vel, acc, dec, smooth;

	for (int i = 21; i < 24; ++i)
	{
		ele = coorToString(i, 2);
		if (i == 21)
		{
			vel = doubleToString(jogModeVariable[motionData->curAxisNum]->velocity);
			jogParCoordinate[ele]->setText(vel);
		}
		else if (i == 22)
		{
			acc = doubleToString(jogModeVariable[motionData->curAxisNum]->acc);
			jogParCoordinate[ele]->setText(acc);
		}
		else
		{
			dec = doubleToString(jogModeVariable[motionData->curAxisNum]->dec);
			jogParCoordinate[ele]->setText(dec);
		}
	}
}

void MotionMode::getJogData()
{
	if (!isGtsOpen)
	{
		QMessageBox::information(
			this,
			tr("Warning"),
			tr("Gts Controller is not opened!"));
		return;
	}

	QString str;
	double vel, acc, dec;

	str = ui.tableWidget->item(21, 1)->text();
	vel = str.toDouble();
	str = ui.tableWidget->item(22, 1)->text();
	acc = str.toDouble();
	str = ui.tableWidget->item(23, 1)->text();
	dec = str.toDouble();

	if (isGtsOpen)
	{
		jogModeVariable[motionData->curAxisNum]->velocity = vel;
		jogModeVariable[motionData->curAxisNum]->acc = acc;
		jogModeVariable[motionData->curAxisNum]->dec = dec;
		jogModeVariable[motionData->curAxisNum]->smooth = 0.5;
	}
}

void MotionMode::enableMotionButton()
{
	long stat;
	int flag, axis, cnt = 0;
	for (int i = 0; i < motionData->correlateAxis.size(); ++i)
	{
		axis = motionData->correlateAxis[i];
		flag = GT_GetSts(axis, &stat);
		gtsCommandHandler("GT_GetSts", axis, flag);
		axisStat = stat;
		if (axisStat.test(9)) cnt++;
	}

	if (cnt == motionData->correlateAxis.size())
	{
		if (jogItem->checkState() == Qt::Checked)
		{
			jogOpt->ui.startButton->setEnabled(true);
			jogOpt->ui.stopButton->setEnabled(true);
		}
		if (ppItem->checkState() == Qt::Checked)
		{
			ppOpt->ui.startButton->setEnabled(true);
			ppOpt->ui.stopButton->setEnabled(true);
		}
	}
	else
	{
		jogOpt->ui.startButton->setEnabled(false);
		jogOpt->ui.stopButton->setEnabled(false);
		ppOpt->ui.startButton->setEnabled(false);
		ppOpt->ui.stopButton->setEnabled(false);
	}
}

void MotionMode::displayPPModeInfo()
{
	QString ele, vel, acc, dec, smooth, delayTm, length, circles;

	for (int i = 25; i < 32; ++i)
	{
		ele = coorToString(i, 2);
		if (i == 25)
		{
			vel = doubleToString(ppModeVariable[motionData->curAxisNum]->velocity);
			ppParCoordinate[ele]->setText(vel);
		}
		else if (i == 26)
		{
			length = doubleToString(ppModeVariable[motionData->curAxisNum]->length);
			ppParCoordinate[ele]->setText(length);
		}
		else if (i == 27)
		{
			acc = doubleToString(ppModeVariable[motionData->curAxisNum]->acc);
			ppParCoordinate[ele]->setText(acc);
		}
		else if (i == 28)
		{
			dec = doubleToString(ppModeVariable[motionData->curAxisNum]->dec);
			ppParCoordinate[ele]->setText(dec);
		}
		else if (i == 29)
		{
			smooth = doubleToString(ppModeVariable[motionData->curAxisNum]->smooth);
			ppParCoordinate[ele]->setText(smooth);
		}
		else if (i == 30)
		{
			circles = doubleToString(ppModeVariable[motionData->curAxisNum]->circles);
			ppParCoordinate[ele]->setText(circles);
		}
		else if (i == 31)
		{
			delayTm = doubleToString(ppModeVariable[motionData->curAxisNum]->delayTm);
			ppParCoordinate[ele]->setText(delayTm);
		}
	}
}

void MotionMode::getPPData()
{
	if (!isGtsOpen)
	{
		QMessageBox::information(
			this,
			tr("Warning"),
			tr("Gts Controller is not opened!"));
		return;
	}

	QString str;
	double vel, acc, dec;
	short smooth, delayTm;
	long length;
	int circles;

	vel = ui.tableWidget->item(25, 1)->text().toDouble();
	length = ui.tableWidget->item(26, 1)->text().toLong();
	acc = ui.tableWidget->item(27, 1)->text().toDouble();
	dec = ui.tableWidget->item(28, 1)->text().toDouble();
	smooth = ui.tableWidget->item(29, 1)->text().toShort();
	circles = ui.tableWidget->item(30, 1)->text().toInt();
	delayTm = ui.tableWidget->item(31, 1)->text().toShort();

	if (isGtsOpen)
	{
		ppModeVariable[motionData->curAxisNum]->velocity = vel;
		ppModeVariable[motionData->curAxisNum]->acc = acc;
		ppModeVariable[motionData->curAxisNum]->dec = dec;
		ppModeVariable[motionData->curAxisNum]->smooth = smooth;
		ppModeVariable[motionData->curAxisNum]->circles = circles;
		ppModeVariable[motionData->curAxisNum]->length = length;
		ppModeVariable[motionData->curAxisNum]->velStart = 5;
		ppModeVariable[motionData->curAxisNum]->delayTm = delayTm;
	}
}

void MotionMode::enableButton()
{
	isGtsOpen = true;
}

void MotionMode::disableButton()
{
	jogOpt->setEnabled(false);
	pidSetting->setEnabled(false);
	ppOpt->setEnabled(false);
}

void MotionMode::enableMotionMode(int x, int y)
{
	QTableWidgetItem *item;
	QString str;

	item = ui.tableWidget->item(x, y);
	if (item == jogItem)
	{
		if (jogItem->checkState() == Qt::Checked)
		{
			jogOpt->setEnabled(true);
			ppOpt->setEnabled(false);
			ppItem->setCheckState(Qt::Unchecked);
			emit jogModeActivated();
			emit ppModeClosed();
		}
		else
		{
			jogOpt->setEnabled(false);
			emit hideAxisMode();
		}
	}
	else if (item == ppItem)
	{
		if (ppItem->checkState() == Qt::Checked)
		{
			ppOpt->setEnabled(true);
			jogOpt->setEnabled(false);
			jogItem->setCheckState(Qt::Unchecked);
			emit ppModeActivated();
			emit jogModeClosed();
		}
		else
		{
			ppOpt->setEnabled(false);
			emit hideAxisMode();
		}
	}

	if (x > 20 && x < 24)
	{
		getJogData();
	}

	if (x > 24 && x < 32)
	{
		getPPData();
	}
}

void MotionMode::loadJogSetting()
{
	if (!isGtsOpen)
	{
		QMessageBox::information(
			this,
			tr("Warning"),
			tr("Gts Controller is not opened!"));
		return;
	}

	if (motionData->correlateAxis.size() == 0)
	{
		QMessageBox::information(
			this,
			tr("Warning"),
			tr("Please select an axis at least!"));
		return;
	}
	
	JogModeVariable tmp;
	QVector<int>::ConstIterator 
		iter = motionData->correlateAxis.cbegin();
	emit infoStart();
	while (iter != motionData->correlateAxis.cend())
	{
		tmp.velocity = jogModeVariable[*iter]->velocity;
		if (tmp.velocity == 0.0)
		{
			settingError(*iter);
			return;
		}
		tmp.acc = jogModeVariable[*iter]->acc;
		tmp.dec = jogModeVariable[*iter]->dec;
		tmp.smooth = 0.5;

		emit sendJogParameter(*iter, &tmp, motionData);
		++iter;
	}
	emit infoEnd();

	enableMotionButton();
}

void MotionMode::loadPPSetting()
{
	if (!isGtsOpen)
	{
		QMessageBox::information(
			this,
			tr("Warning"),
			tr("Gts Controller is not opened!"));
		return;
	}

	if (motionData->correlateAxis.size() == 0)
	{
		QMessageBox::information(
			this,
			tr("Warning"),
			tr("Please select an axis at least!"));
		return;
	}

	PPModeVariable tmp;
	QVector<int>::ConstIterator
		iter = motionData->correlateAxis.cbegin();
	emit infoStart();
	while (iter != motionData->correlateAxis.cend())
	{
		tmp.velocity = ppModeVariable[*iter]->velocity;
		if (tmp.velocity == 0.0)
		{
			settingError(*iter);
			return;
		}
		tmp.acc = ppModeVariable[*iter]->acc;
		tmp.dec = ppModeVariable[*iter]->dec;
		tmp.velStart = ppModeVariable[*iter]->velStart;
		tmp.smooth = ppModeVariable[*iter]->smooth;
		tmp.length = ppModeVariable[*iter]->length;
		tmp.delayTm = ppModeVariable[*iter]->delayTm;
		tmp.circles = ppModeVariable[*iter]->circles;
	

		emit sendPPParameter(*iter, &tmp, motionData);
		++iter;
	}
	emit infoEnd();

	enableMotionButton();
}

void MotionMode::jogStartButton()
{
	if (!foreOrBack)
	{
		jogDirec = true;
		foreOrBack = true;
		jogOpt->ui.startButton->setText("Forward");
	}
	else
	{
		jogDirec = false;
		foreOrBack = false;
		jogOpt->ui.startButton->setText("Backward");
	}

	emit jogMotionDirec(jogDirec);
}

void MotionMode::jogStopButton()
{
	jogOpt->ui.startButton->setText(tr("Start"));
}

void MotionMode::gtsClosed()
{
	jogItem->setCheckState(Qt::Unchecked);
	ppItem->setCheckState(Qt::Unchecked);
}

QString MotionMode::coorToString(int x, int y)
{
	QString str, tmp;
	str.setNum(x);
	tmp.setNum(y);
	str = str + " " + tmp;

	return str;
}

QString MotionMode::doubleToString(double val)
{
	QString str;
	str.setNum(val);

	return str;
}

void MotionMode::settingError(int axis)
{
	QMessageBox::information(
		this,
		tr("Axis Setting Error"),
		tr("The velocity of axis %1 is not set!").arg(axis));
}

void MotionMode::gtsCommandHandler(QString optName, int axis, int flag)
{
	if (flag)
	{
		switch (flag)
		{
		case 1:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 %2 \nExecution of the instruction error").arg(axis).arg(optName));
			break;

		case 2:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 %2 \nGts Controller does not support this feature").arg(axis).arg(optName));
			break;

		case 7:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 %2 \nInstruction parameter error").arg(axis).arg(optName));
			break;

		case -1:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 %2 \nHost and motion controller communication failed").arg(axis).arg(optName));
			break;

		case -6:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 %2 \nFailed to open the controller").arg(axis).arg(optName));
			break;

		case -7:
			QMessageBox::information(
				this,
				tr("Gts command error"),
				tr("Axis %1 %2 \nMotion controller does not respond").arg(axis).arg(optName));
			break;

		default:
			break;
		}
	}
}