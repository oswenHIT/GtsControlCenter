#ifndef MAP_H
#define MAP_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include <qcolor.h>
#include <qstyleoption.h>
#include "ui_map.h"

class MapView : public QGraphicsView
{

};

class Annotation : public QGraphicsItem
{
public:
	 
};

class CityBlock : public QGraphicsItem
{
public:
	enum Kind {
		Park, SmallBuilding, Hospital, Hall, Building, Tower,
		LShapeBlock
	};

	CityBlock(Kind kind);

	QRectF boundingRect() const;
	void paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	int kind;
	QColor color;
	QPainterPath shape;
};

class Map : public QMainWindow
{
	Q_OBJECT

public:
	Map(QWidget *parent = 0);
	~Map();

private:
	Ui::Map ui;
	void generateMap();
	QGraphicsScene *scene;
	MapView *view;
};

#endif // MAP_H
