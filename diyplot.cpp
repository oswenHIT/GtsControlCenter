#include "diyplot.h"

DiyPlot::DiyPlot(QWidget *parent)
	: QwtPlot(parent)
{
	setAutoReplot(false);

	setTitle(tr("Velocity and Position of System"));

	privateCanvas = new QwtPlotCanvas();
	privateCanvas->setBorderRadius(13);
	setCanvas(privateCanvas);
	setCanvasBackground(QColor(0, 0, 139));

	legend = new QwtLegend;
	insertLegend(legend, QwtPlot::BottomLegend);

	enableAxis(QwtPlot::yRight);
	setAxisTitle(QwtPlot::yLeft, tr("Velocity [pulse/s]"));
	setAxisTitle(QwtPlot::yRight, tr("Position [pulse]"));
	setAxisTitle(QwtPlot::xBottom, tr("Time [ms]"));

	planVelocity = new QwtPlotCurve(tr("Plan Velocity"));
	planVelocity->setRenderHint(QwtPlotItem::RenderAntialiased);
	planVelocity->setPen(Qt::yellow);
	planVelocity->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	planVelocity->setYAxis(QwtPlot::yLeft);
	planVelocity->attach(this);

	realVelocity = new QwtPlotCurve(tr("Real Velocity"));
	realVelocity->setRenderHint(QwtPlotItem::RenderAntialiased);
	realVelocity->setPen(Qt::cyan);
	realVelocity->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	realVelocity->setYAxis(QwtPlot::yLeft);
	realVelocity->attach(this);

	planPosition = new QwtPlotCurve(tr("Plan Position"));
	planPosition->setRenderHint(QwtPlotItem::RenderAntialiased);
	planPosition->setPen(QColor(243, 255, 229));
	planPosition->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	planPosition->setYAxis(QwtPlot::yRight);
	planPosition->attach(this);

	realPosition = new QwtPlotCurve(tr("Real Position"));
	realPosition->setRenderHint(QwtPlotItem::RenderAntialiased);
	realPosition->setPen(QColor(205, 145, 158));
	realPosition->setLegendAttribute(QwtPlotCurve::LegendShowLine);
	realPosition->setYAxis(QwtPlot::yRight);
	realPosition->attach(this);

	grid = new QwtPlotGrid;
	grid->enableXMin(true);
	grid->setMajorPen(Qt::white, 0, Qt::DotLine);
	grid->setMinorPen(Qt::gray, 0, Qt::DotLine);
	grid->attach(this);

	setAutoReplot(true);
}

DiyPlot::~DiyPlot()
{

}
