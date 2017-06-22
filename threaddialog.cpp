#include "threaddialog.h"

ThreadDialog::ThreadDialog(QWidget *parent)
	: QDialog(parent)
{
	threadA.setMessage(QString("A"));
	threadA.setMessage(QString("B"));

	threadAButton = new QPushButton(tr("Start A"));
	threadBButton = new QPushButton(tr("Start B"));
	quitButton = new QPushButton(tr("Quit"));
	quitButton->setDefault(true);

	connect(threadAButton, SIGNAL(clicked()), 
		this, SLOT(startOrStopThreadA()));
	connect(threadBButton, SIGNAL(clicked()), 
		this, SLOT(startOrStopThreadB()));
	connect(quitButton, SIGNAL(clicked()),
		this, SLOT(close()));

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(threadAButton);
	mainLayout->addWidget(threadBButton); 
	mainLayout->addWidget(quitButton);

	setLayout(mainLayout);
	setWindowTitle("Thread Test");
}

ThreadDialog::~ThreadDialog()
{

}

void ThreadDialog::startOrStopThreadA()
{
	if (threadA.isRunning())
	{
		threadA.stop();
		threadAButton->setText("Start A");
	}
	else
	{
		threadA.start();
		threadAButton->setText("Stop A");
	}
}

void ThreadDialog::startOrStopThreadB()
{
	if (threadB.isRunning())
	{
		threadB.stop();
		threadBButton->setText("Start B");
	}
	else
	{
		threadB.start();
		threadBButton->setText("Stop B");
	}
}

void ThreadDialog::closeEvent(QCloseEvent *cevent)
{
	threadA.stop();
	threadB.stop();
	threadA.wait();
	threadB.wait();

	cevent->accept();
}