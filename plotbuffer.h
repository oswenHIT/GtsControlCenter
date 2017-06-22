#ifndef PLOTBUFFER_H
#define PLOTBUFFER_H

#include <QObject>

class PlotBuffer : public QObject
{
	Q_OBJECT

public:
	PlotBuffer(QObject *parent);
	~PlotBuffer();

private:
	
};

#endif // PLOTBUFFER_H
