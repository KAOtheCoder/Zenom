#include "plotmagnifier.h"

PlotMagnifier::PlotMagnifier(QWidget *pCanvas) :
    QwtPlotMagnifier(pCanvas)
{
}

void PlotMagnifier::rescale(double factor)
{
    emit zoomed();
    QwtPlotMagnifier::rescale(factor);
}
