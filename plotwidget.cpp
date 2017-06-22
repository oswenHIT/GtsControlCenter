#include "plotwidget.h"

#define PI 3.1415

extern QVector<double> xdata;
extern QVector<double> prfVel[4];
extern QVector<double> encVel[4];
extern QVector<double> posError[4];
extern QReadWriteLock mutex;

PlotWidget::PlotWidget(QMainWindow *window, QWidget *widget)
	: QWidget(widget)
{
	ui.setupUi(this);

	plotMap[0] = ui.customPlot1;
	plotMap[1] = ui.customPlot2;
	plotMap[2] = ui.customPlot3;
	plotMap[3] = ui.customPlot4;
	for (int i = 0; i < 4; ++i)
	{
		axisDblClc[i] = false;
		zoomMode[i] = false;
		mode[i] = tr("[X Zoom]");
//		text[i] = new QCPItemText(plotMap[i]);
//		text[i]->setText("Test");
	}
	trackMode = true;
	curXAxis = 0;
	curPos = 0;
	curSize = 0;
	mainWindow = window;
	timer = new QTimer;

	mutex.lockForWrite();
	xdata << 0;
	for (int i = 0; i < 4; ++i)
	{
		prfVel[i] << 0;
		encVel[i] << 0;
		posError[i] << 0;
	}
	mutex.unlock();

	initFileDialog();
	initCustomPlot1();
	initCustomPlot2();
	initCustomPlot3();
	initCustomPlot4();

	connect(ui.clearButton, &QPushButton::clicked, 
		this, &PlotWidget::clearData);
	connect(ui.saveButton, &QPushButton::clicked, 
		this, &PlotWidget::savePicture);
	connect(ui.axisOneButton, &QPushButton::clicked, 
		this, &PlotWidget::axisOneVisible);
	connect(ui.axisTwoButton, &QPushButton::clicked, 
		this, &PlotWidget::axisTwoVisible);
	connect(ui.axisThreeButton, &QPushButton::clicked, 
		this, &PlotWidget::axisThreeVisible);
	connect(ui.axisFourButton, &QPushButton::clicked, 
		this, &PlotWidget::axisFourVisible);
	connect(ui.trackCheckBox, &QCheckBox::stateChanged,
		this, &PlotWidget::trackModeEnable);
	connect(timer, &QTimer::timeout, this, &PlotWidget::updatePlot);

	ui.saveButton->setVisible(false);
	ui.saveButton->setStatusTip(tr("Save current axis picture"));
	ui.clearButton->setStatusTip(tr("Clear all axis plot data"));
	ui.trackCheckBox->setStatusTip(tr("Activate track mode?"));
}

PlotWidget::~PlotWidget()
{

}

void PlotWidget::initCustomPlot1()
{
	QPen pen;
	pen.setWidthF(2);
	pen.setColor(QColor(255, 0, 0));

	ui.customPlot1->setNoAntialiasingOnDrag(true);
	ui.customPlot1->addGraph();
	ui.customPlot1->graph(0)->setName(tr("Plan Velocity"));
	ui.customPlot1->graph(0)->setBrush(QBrush(QColor(0, 205, 205, 60)));
	tracer[0] = new QCPItemTracer(ui.customPlot1);
	ui.customPlot1->addItem(tracer[0]);
	tracer[0]->setStyle(QCPItemTracer::tsCircle);
	tracer[0]->setSize(4);
	tracer[0]->setPen(QColor(0, 0, 255));
	tracer[0]->setBrush(QBrush(QColor(0, 0, 255)));
	tracer[0]->setGraph(ui.customPlot1->graph(0));
	ui.customPlot1->addGraph();
	ui.customPlot1->graph(1)->setName(tr("Real Velocity"));
	ui.customPlot1->graph(1)->setBrush(QBrush(QColor(238, 18, 137, 60)));
	ui.customPlot1->graph(1)->setPen(pen);
	tracer[4] = new QCPItemTracer(ui.customPlot1);
	ui.customPlot1->addItem(tracer[4]);
	tracer[4]->setStyle(QCPItemTracer::tsCircle);
	tracer[4]->setSize(5);
	tracer[4]->setPen(pen);
	tracer[4]->setBrush(QBrush(QColor(255, 0, 0)));
	tracer[4]->setGraph(ui.customPlot1->graph(1));
	ui.customPlot1->addGraph();
	ui.customPlot1->graph(2)->setName(tr("Track Point"));
	tracer[8] = new QCPItemTracer(ui.customPlot1);
	ui.customPlot1->addItem(tracer[4]);
	tracer[8]->setPen(QColor(255, 105, 180));
	tracer[8]->setStyle(QCPItemTracer::tsNone);
	tracer[8]->setGraph(ui.customPlot1->graph(2));
	tracer[8]->position->setType(QCPItemPosition::ptPlotCoords);
	ui.customPlot1->xAxis->setRange(0, 5000, Qt::AlignLeft);
	ui.customPlot1->xAxis->setLabel(tr("Time [ ms ]"));
	ui.customPlot1->xAxis->setTickStep(0.1);
	ui.customPlot1->yAxis->setTickStep(0.01);
	ui.customPlot1->xAxis->setAutoTicks(true);
	ui.customPlot1->yAxis->setRange(0, 800, Qt::AlignHCenter);
	ui.customPlot1->yAxis->setLabel(tr("Plan Velocity [ pulse/ms ]"));
	ui.customPlot1->legend->setFont(QFont());
	ui.customPlot1->legend->setVisible(true);
	ui.customPlot1->plotLayout()->insertRow(0);
	ui.customPlot1->plotLayout()->addElement(
		0, 0, new QCPPlotTitle(ui.customPlot1, tr("Axis One")));
	ui.customPlot1->setInteraction(QCP::iRangeDrag, true);
	ui.customPlot1->setInteraction(QCP::iRangeZoom, true);
	ui.customPlot1->axisRect()->setRangeZoomAxes(ui.customPlot1->xAxis, 0);
	ui.customPlot1->installEventFilter(this);
	ui.customPlot1->replot();
}

void PlotWidget::initCustomPlot2()
{
	QPen pen;
	pen.setWidthF(2);
	pen.setColor(QColor(255, 0, 0));

	ui.customPlot2->addGraph();
	ui.customPlot2->graph(0)->setName(tr("Plan Velocity"));
	ui.customPlot2->graph(0)->setBrush(QBrush(QColor(0, 205, 205, 60)));
	tracer[1] = new QCPItemTracer(ui.customPlot2);
	ui.customPlot2->addItem(tracer[1]);
	tracer[1]->setStyle(QCPItemTracer::tsCircle);
	tracer[1]->setSize(4);
	tracer[1]->setPen(QColor(0, 0, 255));
	tracer[1]->setBrush(QBrush(QColor(0, 0, 255)));
	tracer[1]->setGraph(ui.customPlot2->graph(0));
	ui.customPlot2->addGraph();
	ui.customPlot2->graph(1)->setName(tr("Real Velocity"));
	ui.customPlot2->graph(1)->setBrush(QBrush(QColor(50, 205, 50, 60)));
	ui.customPlot2->graph(1)->setPen(pen);
	tracer[5] = new QCPItemTracer(ui.customPlot2);
	ui.customPlot2->addItem(tracer[5]);
	tracer[5]->setStyle(QCPItemTracer::tsCircle);
	tracer[5]->setSize(5);
	tracer[5]->setPen(pen);
	tracer[5]->setBrush(QBrush(QColor(255, 0, 0)));
	tracer[5]->setGraph(ui.customPlot2->graph(1));
	ui.customPlot2->addGraph();
	ui.customPlot2->graph(2)->setName(tr("Track Point"));
	tracer[9] = new QCPItemTracer(ui.customPlot2);
	ui.customPlot2->addItem(tracer[9]);
	tracer[9]->setPen(QColor(255, 105, 180));
	tracer[9]->setStyle(QCPItemTracer::tsNone);
	tracer[9]->setGraph(ui.customPlot2->graph(2));
	tracer[9]->position->setType(QCPItemPosition::ptPlotCoords);
	ui.customPlot2->xAxis->setRange(0, 5000, Qt::AlignLeft);
	ui.customPlot2->xAxis->setLabel(tr("Time [ ms ]"));
	ui.customPlot2->xAxis->setTickStep(0.1);
	ui.customPlot2->yAxis->setTickStep(0.01);
	ui.customPlot2->xAxis->setAutoTicks(true);
	ui.customPlot2->yAxis->setRange(0, 800, Qt::AlignHCenter);
	ui.customPlot2->yAxis->setLabel(tr("Velocity [ pulse/ms ]"));

	ui.customPlot2->legend->setFont(QFont());
	ui.customPlot2->legend->setVisible(true);

	ui.customPlot2->plotLayout()->insertRow(0);
	ui.customPlot2->plotLayout()->addElement(
		0, 0, new QCPPlotTitle(ui.customPlot2, tr("Axis Two")));
	ui.customPlot2->setInteraction(QCP::iRangeDrag, true);
	ui.customPlot2->setInteraction(QCP::iRangeZoom, true);
	ui.customPlot2->axisRect()->setRangeZoomAxes(ui.customPlot2->xAxis, 0);
	ui.customPlot2->installEventFilter(this);
	ui.customPlot2->replot();
}

void PlotWidget::initCustomPlot3()
{
	QPen pen;
	pen.setWidthF(2);
	pen.setColor(QColor(255, 0, 0));

	ui.customPlot3->addGraph();
	ui.customPlot3->graph(0)->setName(tr("Plan Velocity"));
	ui.customPlot3->graph(0)->setBrush(QBrush(QColor(0, 205, 205, 60)));
	tracer[2] = new QCPItemTracer(ui.customPlot3);
	ui.customPlot3->addItem(tracer[2]);
	tracer[2]->setStyle(QCPItemTracer::tsCircle);
	tracer[2]->setSize(5);
	tracer[2]->setPen(QColor(0, 0, 255));
	tracer[2]->setBrush(QBrush(QColor(0, 0, 255)));
	tracer[2]->setGraph(ui.customPlot3->graph(0));
	ui.customPlot3->addGraph();
	ui.customPlot3->graph(1)->setName(tr("Real Velocity"));
	ui.customPlot3->graph(1)->setBrush(QBrush(QColor(255, 218, 185, 60)));
	ui.customPlot3->graph(1)->setPen(pen);
	tracer[6] = new QCPItemTracer(ui.customPlot3);
	ui.customPlot3->addItem(tracer[6]);
	tracer[6]->setStyle(QCPItemTracer::tsCircle);
	tracer[6]->setSize(5);
	tracer[6]->setPen(pen);
	tracer[6]->setBrush(QBrush(QColor(255, 0, 0)));
	tracer[6]->setGraph(ui.customPlot3->graph(1));
	ui.customPlot3->addGraph();
	ui.customPlot3->graph(2)->setName(tr("Track Point"));
	tracer[10] = new QCPItemTracer(ui.customPlot3);
	ui.customPlot3->addItem(tracer[10]);
	tracer[10]->setPen(QColor(255, 105, 180));
	tracer[10]->setStyle(QCPItemTracer::tsNone);
	tracer[10]->setGraph(ui.customPlot3->graph(2));
	tracer[10]->position->setType(QCPItemPosition::ptPlotCoords);
	ui.customPlot3->xAxis->setRange(0, 5000, Qt::AlignLeft);
	ui.customPlot3->xAxis->setLabel(tr("Time [ ms ]"));
	ui.customPlot3->xAxis->setTickStep(0.1);
	ui.customPlot3->yAxis->setTickStep(0.01);
	ui.customPlot3->xAxis->setAutoTicks(true);
	ui.customPlot3->yAxis->setRange(0, 800, Qt::AlignHCenter);
	ui.customPlot3->yAxis->setLabel(tr("Velocity [ pulse/ms ]"));

	ui.customPlot3->legend->setFont(QFont());
	ui.customPlot3->legend->setVisible(true);

	ui.customPlot3->plotLayout()->insertRow(0);
	ui.customPlot3->plotLayout()->addElement(
		0, 0, new QCPPlotTitle(ui.customPlot3, tr("Axis Three")));
	ui.customPlot3->setInteraction(QCP::iRangeDrag, true);
	ui.customPlot3->setInteraction(QCP::iRangeZoom, true);
	ui.customPlot3->axisRect()->setRangeZoomAxes(ui.customPlot3->xAxis, 0);
	ui.customPlot3->installEventFilter(this);
	ui.customPlot3->replot();
}

void PlotWidget::initCustomPlot4()
{
	QPen pen;
	pen.setWidthF(2);
	pen.setColor(QColor(255, 0, 0));

	ui.customPlot4->addGraph();
	ui.customPlot4->graph(0)->setName(tr("Plan Velocity"));
	ui.customPlot4->graph(0)->setBrush(QBrush(QColor(0, 205, 205, 60)));
	tracer[3] = new QCPItemTracer(ui.customPlot4);
	ui.customPlot4->addItem(tracer[3]);
	tracer[3]->setStyle(QCPItemTracer::tsCircle);
	tracer[3]->setSize(4);
	tracer[3]->setPen(QColor(0, 0, 255));
	tracer[3]->setBrush(QBrush(QColor(0, 0, 255)));
	tracer[3]->setGraph(ui.customPlot4->graph(0));
	ui.customPlot4->addGraph();
	ui.customPlot4->graph(1)->setName(tr("Real Velocity"));
	ui.customPlot4->graph(1)->setBrush(QBrush(QColor(160, 32, 240, 60)));
	ui.customPlot4->graph(1)->setPen(pen);
	tracer[7] = new QCPItemTracer(ui.customPlot4);
	ui.customPlot4->addItem(tracer[7]);
	tracer[7]->setStyle(QCPItemTracer::tsCircle);
	tracer[7]->setSize(5);
	tracer[7]->setPen(pen);
	tracer[7]->setBrush(QBrush(QColor(255, 0, 0)));
	tracer[7]->setGraph(ui.customPlot4->graph(1));
	ui.customPlot4->addGraph();
	ui.customPlot4->graph(2)->setName(tr("Track Point"));
	tracer[11] = new QCPItemTracer(ui.customPlot4);
	ui.customPlot4->addItem(tracer[11]);
	tracer[11]->setPen(QColor(255, 105, 180));
	tracer[11]->setStyle(QCPItemTracer::tsNone);
	tracer[11]->setGraph(ui.customPlot4->graph(2));
	tracer[11]->position->setType(QCPItemPosition::ptPlotCoords);
	ui.customPlot4->xAxis->setRange(0, 5000, Qt::AlignLeft);
	ui.customPlot4->xAxis->setLabel(tr("Time [ ms ]"));
	ui.customPlot4->xAxis->setTickStep(0.1);
	ui.customPlot4->yAxis->setTickStep(0.01);
	ui.customPlot4->xAxis->setAutoTicks(true);
	ui.customPlot4->yAxis->setRange(0, 800, Qt::AlignHCenter);
	ui.customPlot4->yAxis->setLabel(tr("Velocity [ pulse/ms ]"));

	ui.customPlot4->legend->setFont(QFont());
	ui.customPlot4->legend->setVisible(true);

	ui.customPlot4->plotLayout()->insertRow(0);
	ui.customPlot4->plotLayout()->addElement(
		0, 0, new QCPPlotTitle(ui.customPlot4, tr("Axis Four")));
	ui.customPlot4->setInteraction(QCP::iRangeDrag, true);
	ui.customPlot4->setInteraction(QCP::iRangeZoom, true);
	ui.customPlot4->axisRect()->setRangeZoomAxes(ui.customPlot4->xAxis, 0);
	ui.customPlot4->installEventFilter(this);
	ui.customPlot4->replot();
}

void PlotWidget::initFileDialog()
{
	QStringList filters;
	filters << "PDF (*.pdf)"
			<< "JPG Image (*.jpg)"
			<< "PNG Image (*.png)"
			<< "BMP Image (*.bmp)";

	saveFile = new QFileDialog;
	saveFile->setWindowTitle(tr("Save Axis Image"));
	saveFile->setFileMode(QFileDialog::AnyFile);
	saveFile->setAcceptMode(QFileDialog::AcceptSave);
	saveFile->setNameFilters(filters);
	saveFile->setDefaultSuffix("jpg");
	saveFile->setDirectory(QDir("F:/"));
}

void PlotWidget::axisOneVisible()
{
	for (int i = 0; i < 4; ++i)
	{
		if (i != 0)
		{
			plotMap[i]->setVisible(false);
			axisDblClc[i] = false;
		}
		else
		{
			plotMap[i]->setVisible(true);
			axisDblClc[i] = true;
		}
	}
	ui.saveButton->setVisible(true);
}

void PlotWidget::axisTwoVisible()
{
	for (int i = 0; i < 4; ++i)
	{
		if (i != 1)
		{
			plotMap[i]->setVisible(false);
			axisDblClc[i] = false;
		}
		else
		{
			plotMap[i]->setVisible(true);
			axisDblClc[i] = true;
		}
	}
	ui.saveButton->setVisible(true);
}

void PlotWidget::axisThreeVisible()
{
	for (int i = 0; i < 4; ++i)
	{
		if (i != 2)
		{
			plotMap[i]->setVisible(false);
			axisDblClc[i] = false;
		}
		else
		{
			plotMap[i]->setVisible(true);
			axisDblClc[i] = true;
		}
	}
	ui.saveButton->setVisible(true);
}

void PlotWidget::axisFourVisible()
{
	for (int i = 0; i < 4; ++i)
	{
		if (i != 3)
		{
			plotMap[i]->setVisible(false);
			axisDblClc[i] = false;
		}
		else
		{
			plotMap[i]->setVisible(true);
			axisDblClc[i] = true;
		}
	}
	ui.saveButton->setVisible(true);
}

void PlotWidget::receiveModeData(toMotionModeData *data)
{
	axisData = data;
	axisQueue.clear();
	QVector<int>::const_iterator iter = axisData->correlateAxis.cbegin();
	while (iter != axisData->correlateAxis.cend())
	{
		axisQueue.push_back(*iter);
		++iter;
	}
	axisIter = axisQueue.cbegin();
}

void PlotWidget::savePicture()
{
	bool rtn;
	QStringList imgNames;
	QString imgName;
	QString suffix;

	if (saveFile->exec())
	{
		imgNames = saveFile->selectedFiles();
		if (imgNames.size() > 0)
		{
			QFileInfo info(imgNames.first());
			imgName = imgNames.first();
			suffix = info.suffix();
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		if (axisDblClc[i])
		{
			suffix == "jpg" ?
				rtn = plotMap[i]->saveJpg(imgName) :
			suffix == "pdf" ?
				rtn = plotMap[i]->savePdf(imgName) :
			suffix == "png" ?
				rtn = plotMap[i]->savePng(imgName) :
			suffix == "bmp" ?
				rtn = plotMap[i]->saveBmp(imgName) :
			&axisDblClc;
		}
	}

	if (rtn)
	{
		mainWindow->statusBar()->showMessage(tr("Saving Image..."), 12000);
	}
	else
	{
		QMessageBox::information(
			mainWindow, 
			tr("Save error"), 
			tr("Can not save image!"));
	}
}

void PlotWidget::clearData()
{
	timer->stop();

	mutex.lockForWrite();
	xdata.clear();
	for (int i = 0; i < 4; i++)
	{
		plotMap[i]->graph(0)->clearData();
		plotMap[i]->graph(1)->clearData();
		prfVel[i].clear();
		encVel[i].clear();
		plotMap[i]->replot();
	}
	mutex.unlock();
}

void PlotWidget::startPlot()
{
	for (int i = 0; i < 4; i++)
	{
		plotMap[i]->setInteraction(QCP::iRangeDrag, false);
		plotMap[i]->axisRect()->setRangeZoomAxes(plotMap[i]->xAxis, 0);
	}

	trackMode = false;
	ui.trackCheckBox->setCheckState(Qt::Unchecked);
	ui.trackCheckBox->setEnabled(false);
	ui.saveButton->setEnabled(false);
	ui.clearButton->setEnabled(false);

	timer->start(6);
}



void PlotWidget::stopPlot()
{
	timer->stop();

	QReadLocker locker(&mutex);
	for (int i = 0; i < 4; i++)
	{
		plotMap[i]->setInteraction(QCP::iRangeDrag, true);
		if (xdata.size() != 0)
			curPos = xdata.back() - plotMap[i]->xAxis->range().size()*0.13;
		curSize = plotMap[i]->xAxis->range().size();
	}

	trackMode = true;
	ui.saveButton->setEnabled(true);
	ui.clearButton->setEnabled(true);
	ui.trackCheckBox->setEnabled(true);
}

int PlotWidget::determineAxis()
{
	int curAxis;

	if (axisIter == axisQueue.cend())
	{
		axisIter = axisQueue.cbegin();
	}

	curAxis = *axisIter;
	++axisIter;

	return curAxis-1;
}

void PlotWidget::updatePlot()
{	
	int axis = determineAxis();

	if (xdata.size() == 0)
	{
		return;
	}

	mutex.lockForRead();
	plotMap[axis]->graph(0)->setData(xdata, prfVel[axis]);
	plotMap[axis]->graph(1)->setData(xdata, encVel[axis]);
	plotMap[axis]->xAxis->setRange(
		xdata.back() - plotMap[axis]->xAxis->range().size()*0.13,
		plotMap[axis]->xAxis->range().size(),
		Qt::AlignCenter);
	tracer[axis]->setGraphKey(xdata.back());
	tracer[axis+4]->setGraphKey(xdata.back());
	plotMap[axis]->replot();
	mutex.unlock();
}

bool PlotWidget::eventFilter(QObject *obj, QEvent *ev)
{
	for (int i = 1; i <= 4; ++i)
	{
		if (obj == plotMap[i - 1])
		{
			if (ev->type() == QEvent::MouseMove)
			{
				QMouseEvent *mevent = static_cast<QMouseEvent*>(ev);
				showInfo(mevent->pos(), i);
			}
			else if (ev->type() == QEvent::MouseButtonPress)
			{
				QMouseEvent *mevent = static_cast<QMouseEvent*>(ev);
				if (mevent->button() == Qt::RightButton)
				{
					if (!zoomMode[i - 1])
					{
						plotMap[i - 1]->axisRect()->setRangeZoomAxes(
							plotMap[i - 1]->xAxis,
							plotMap[i - 1]->yAxis);
						zoomMode[i - 1] = true;
						mode[i - 1] = tr("[X-Y Zoom]");
					}
					else
					{
						plotMap[i - 1]->axisRect()->setRangeZoomAxes(
							plotMap[i - 1]->xAxis,
							0);
						zoomMode[i - 1] = false;
						mode[i - 1] = tr("[X Zoom]");
					}

					showInfo(mevent->pos(), i);
					return true;
				}
			}
			else if (ev->type() == QEvent::MouseButtonDblClick)
			{
				QMouseEvent *mevent = static_cast<QMouseEvent*>(ev);
				if (mevent->button() == Qt::LeftButton)
				{
					if (!axisDblClc[i - 1])
					{
						for (int j = 1; j <= 4; ++j)
						{
							if (j != i)
							{
								plotMap[j - 1]->setVisible(false);
							}
						}
						axisDblClc[i - 1] = true;
						ui.saveButton->setVisible(true);
					}
					else
					{
						for (int j = 1; j <= 4; ++j)
						{
							plotMap[j - 1]->setVisible(true);
							axisDblClc[j - 1] = false;
						}
						ui.saveButton->setVisible(false);
					}

					return true;
				}
				else if (mevent->button() == Qt::RightButton)
				{
					plotMap[i - 1]->axisRect()->setRangeZoomAxes(
						plotMap[i - 1]->xAxis,
						0);
					zoomMode[i - 1] = false;
					mode[i - 1] = tr("[X Zoom]");

					plotMap[i - 1]->yAxis->setRange(0, 800, Qt::AlignCenter);
					plotMap[i - 1]->xAxis->setRange(curPos, curSize, Qt::AlignCenter);
					plotMap[i - 1]->replot();

					showInfo(mevent->pos(), i);
					return true;
				}
			}
		}
	}
	
	return QObject::eventFilter(obj, ev);
}

void PlotWidget::showInfo(QPoint pos, int axis)
{
	QString info, num;
	QCPItemPosition textPos(plotMap[axis - 1], text[axis - 1], "Test");
	QPointF coordinate;
	coordinate.setX(plotMap[axis - 1]->xAxis->pixelToCoord(pos.x()));
	coordinate.setY(plotMap[axis - 1]->yAxis->pixelToCoord(pos.y()));

	info = tr("Axis[%1]  [X: ").arg(axis);
	num.setNum(coordinate.x());
	info = info + num + ", Y:";
	num.setNum(coordinate.y());
	info = info + num;
	info = info + "]";
	info = info + " " + mode[axis - 1];

	if (trackMode)
	{
		tracer[axis + 7]->position->setCoords(coordinate);
		plotMap[axis - 1]->replot();
	}
	mainWindow->statusBar()->showMessage(info);
	ui.infoLabel->setText(info);
}

void PlotWidget::trackModeEnable()
{
	QCPItemTracer::TracerStyle style;
	ui.trackCheckBox->isChecked() ? 
		style = QCPItemTracer::tsCrosshair : 
		style = QCPItemTracer::tsNone;

	for (int i = 0; i < 4; ++i)
	{
		tracer[i+8]->setStyle(style);
		plotMap[i]->replot();
	}
}