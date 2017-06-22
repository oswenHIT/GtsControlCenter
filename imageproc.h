#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <xCComDev.h>
#include "ui_imageproc.h"

class ImageProc : public QWidget
{
	Q_OBJECT

public:
	ImageProc(QWidget *parent = 0);
	~ImageProc();


public:
	void buildCommunication();
	void closeLight() const;

private:
	void selectComPort(const QString &text);
	void activateButton();
	void connectComPort();
	void setCurChannel(int i);
	void setCurIntensity(int i);
	void setIntensity();
	void getIntensity(); 
	void openLight() const;
	void getFirmVersion();
	void getPorts();
	void recheckPorts();

private:
	Ui::ImageProc ui;

	QList<QSerialPortInfo> ports;

	int curChannel;
	int curIntensity;
	bool flag;
};

#endif // IMAGEPROC_H
