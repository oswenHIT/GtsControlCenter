#ifndef AXISPROPERTYSHEET_H
#define AXISPROPERTYSHEET_H

#include <qwidget.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qvalidator.h>
#include <PropertyBrowser/qtpropertybrowser.h>
#include <PropertyBrowser/qtpropertymanager.h>
#include <PropertyBrowser/qttreepropertybrowser.h>
#include <PropertyBrowser/qteditorfactory.h>

class AxisPropertySheet : public QWidget
{
	Q_OBJECT

public:
	AxisPropertySheet(QWidget *parent = 0);
	~AxisPropertySheet();
	void createPropertySheet();

private:
	QtTreePropertyBrowser *propertyBrowser;
	QPushButton *resetButton;
	QPushButton *loadSetting;
};

#endif // AXISPROPERTYSHEET_H
