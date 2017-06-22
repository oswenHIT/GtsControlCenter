#ifndef CAMERA_H
#define CAMERA_H


//Qt header
#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QCloseEvent>
#include <QTimer>
#include <QTime>
#include <QProcess>
#include <QMdiArea>
#include <QFileDialog>

//STL and Imaging header file
#include <tisudshl.h>
#include <vector>
#include <memory>

//OpenCV header file
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//DIY Qt widgets header file
#include "motionmode.h"
#include "axispropertysheet.h"
#include "axisoperate.h"
#include "diydockwidget.h"
#include "ui_camera.h"
#include "Listener.h"
#include "assisstclass.h"
#include "WrFilter.h"


using namespace _DSHOWLIB_NAMESPACE;


class Camera : public QMainWindow
{
	Q_OBJECT

public:
	Camera(QWidget *parent = 0);
	~Camera();

public://Display function
	void enableFullScreen();
	void toNormalMode();
	void minimizeApp();
	void exitApp();

public://Slots functions
	void openCamera();
	void closeCamera();
	void pauseCamera();
	void openSetting();
	void openDevice();
	void snapImage();
	void changeCurCamera(int index);
	void openDriveProg();
	void setDrivePath();
	void setGrabMode();
	void enableHistEQL(int stat);
	void setFilterMode(int mode);

public://Action slots
	void imageFlipH(bool);
	void imageFlipV(bool);
	void timerSlots();
	void aboutQT();

public://Application events
	void closeEvent(QCloseEvent *ev);
	void keyPressEvent(QKeyEvent *ev);

private://Inner error message box
	void buildCommunication();
	void initCamera();
	void createSinkListener();
	void errorMsgBox();
	bool addCameraItem();
	void createDockWidget();

private://Ui member
	Ui::Camera ui;
	QTimer* timer;
	QProcess* process;
	AxisPropertySheet * sheet;
	MotionMode * mode;
	QMdiArea * mdiArea;
	AxisOperate * axisOpt;
	QMdiArea * centralMdi;

private://Industrial camera members
	Grabber mGrabber;
	DShowLib::tFrameHandlerSinkPtr pSink;
	smart_ptr<std::vector<DShowLib::VideoCaptureDeviceItem>> pDevList;
	CListener *listner;
	FrameTypeInfo fType;
	BYTE* buffer[10];
	DShowLib::tMemBufferCollectionPtr pCollection;
	smart_com<WRFilter> pFilter;

private://Inner data members
	int curIndex;
	int curCount;
	char isDeleted;
	bool isInit;
	bool isPause;
	bool isGrabMode;
	QString drivePath;
	wr::ROI roi;
	enum delStat {original, notyet, deleted};
};


#endif // CAMERA_H
