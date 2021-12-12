#ifndef LEGEND_H
#define LEGEND_H

#include <QwtAbstractLegend>
#include <QwtLegendData>
#include <QwtPlotItem>

#include "legenditem.h"

class Legend : public QwtAbstractLegend
{
    Q_OBJECT

public:
    Legend(QWidget* parent = nullptr);

    void renderLegend(QPainter *, const QRectF &, bool) const override {}
    bool isEmpty() const override { return m_legendItems.isEmpty(); }

public slots:
    void updateLegend(const QVariant &itemInfo, const QList< QwtLegendData > &data) override;

private:
    QMap<QwtPlotItem*, LegendItem*> m_legendItems;
    QLayout* m_legendLayout;
};

#endif // LEGEND_H
