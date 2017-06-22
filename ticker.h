#ifndef TICKER_H
#define TICKER_H

#include <qevent.h>
#include <QPaintEvent>
#include <qwidget.h>
#include <qtimer.h>
#include <QPainter>
#include <QtWidgets/qlineedit.h>

class Ticker : public QWidget
{
	Q_OBJECT;

public:
	Ticker(QWidget *parent = 0) : QWidget(parent) 
	{
		offset = 0;
		myTimerID = 0;

		edit->installEventFilter(this);
	}

	void setText(const QString &newText);
	QString text() const { return myText; }
	QSize sizeHint() const;

protected:
	void paintEvent(QPaintEvent *pevent);
	void timerEvent(QTimerEvent *tevent);
	void showEvent(QShowEvent *sevent);
	void hideEvent(QHideEvent *hevent);
	bool eventFilter(QObject *target, QEvent *qevent);

private:
	QString myText;
	int offset;
	int myTimerID;
	QLineEdit *edit;
};

#endif