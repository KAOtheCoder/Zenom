#include "legend.h"
#include "plot.h"
#include "../widget/unboundedscrollarea.h"

#include <QVBoxLayout>
#include <QVariant>
#include <QDebug>
#include <QwtPlotCurve>
#include <QwtText>
#include <QPen>

Legend::Legend(QWidget* parent)
    : QwtAbstractLegend(parent),
      m_legendLayout(new QVBoxLayout())
{
    auto legendWidget = new QWidget();
    m_legendLayout->setAlignment(Qt::AlignTop);
    legendWidget->setLayout(m_legendLayout);

    auto scrollArea = new UnboundedScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(legendWidget);
    legendWidget->setAutoFillBackground(false);
    scrollArea->viewport()->setAutoFillBackground(false);

    setLayout(new QVBoxLayout());
    layout()->addWidget(scrollArea);
}

void Legend::updateLegend (const QVariant &itemInfo, const QList< QwtLegendData > &pData) {
    Q_UNUSED(pData);

    auto item = itemInfo.value<QwtPlotItem*>();

    if (item->rtti() == QwtPlotItem::Rtti_PlotCurve) {
        auto legendItem = m_legendItems.value(item);

        if (!legendItem) {
            auto curve = static_cast<PlotCurve*>(item);
            const auto& name = curve->title().text();
            legendItem = new LegendItem();
            legendItem->setName(name);
            legendItem->setColor(curve->pen().color());
            legendItem->setSize(curve->pen().width());
            m_legendLayout->addWidget(legendItem);
            m_legendItems.insert(item, legendItem);
            auto plot = static_cast<Plot*>(curve->plot());

            connect(legendItem, &LegendItem::removeRequested, [&, plot, curve, item]() {
                plot->removeCurve(curve);
                m_legendItems.take(item)->deleteLater();
            });

            connect(legendItem, &LegendItem::colorChanged, [curve](const QColor& pColor) { curve->setColor(pColor); });
            connect(legendItem, &LegendItem::widthChanged, [curve](const int pWidth) { curve->setWidth(pWidth); });
        }
    }
}
