#include <QtWidgets\qapplication.h>
#include <opencv2/core/core.hpp>
#include <opencv.hpp>
#include <cv.hpp>
#include <iostream>
#include <QDebug>
#include "mainwindow.h"
#include "camera.h"

using namespace cv;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTranslator appTranslator;
	appTranslator.load("gtscontrolcenter_zh.qm");
	app.installTranslator(&appTranslator);
	
//	MainWindow *mainWindow = new MainWindow;
//	mainWindow->show();
	auto cam = new Camera;
	//cam->enableFullScreen();
	cam->show();
	return app.exec();
}
