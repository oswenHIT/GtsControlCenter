#include "map.h"

Map::Map(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	scene = new QGraphicsScene(-22.25, -22.25, 1980, 1980);
	scene->setBackgroundBrush(QColor(255, 255, 238));
	generateMap();

	view = new MapView;
	view->setScene(scene);
	setCentralWidget(view);
}

Map::~Map()
{

}

void Map::generateMap()
{

}

void CityBlock::paint(QPainter *painter,
	const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (option->levelOfDetail < 4.0)
	{
		painter->fillPath(shape, color);
	}
	else
	{
		QLinearGradient gradient(QPoint(-20, -20), QPoint(+20, +20));
		int coeff = 105 + int(std::log(option->levelOfDetail - 4.0));
		gradient.setColorAt(0.0, color.lighter(coeff));
		gradient.setColorAt(1.0, color.darker(coeff));
		painter->fillPath(shape, gradient);
	}
}