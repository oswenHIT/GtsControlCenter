#ifndef DIYPLOT_H
#define DIYPLOT_H

#include <QWT/qwt_math.h>
#include <QWT/qwt_scale_engine.h>
#include <QWT/qwt_symbol.h>
#include <QWT/qwt_plot_grid.h>
#include <QWT/qwt_plot_marker.h>
#include <QWT/qwt_plot_curve.h>
#include <QWT/qwt_legend.h>
#include <QWT/qwt_text.h>
#include <QWT/qwt_plot_canvas.h>
#include <QWT/qwt_plot.h>
#include <QWT/qwt_counter.h>
#include <QWT/qwt_picker_machine.h>
#include <QWT/qwt_plot_zoomer.h>
#include <QWT/qwt_plot_panner.h>
#include <QWT/qwt_plot_renderer.h>

class DiyPlot : public QwtPlot
{
	Q_OBJECT

public:
	DiyPlot(QWidget *parent = 0);
	~DiyPlot();

private:
	QwtPlotCurve *planVelocity;
	QwtPlotCurve *realVelocity;
	QwtPlotCurve *planPosition;
	QwtPlotCurve *realPosition;
	QwtPlotCanvas *privateCanvas;
	QwtLegend *legend;
	QwtPlotGrid *grid;
};

#endif // DIYPLOT_H
