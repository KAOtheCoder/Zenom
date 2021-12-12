#include "unboundedscrollarea.h"

#include <QScrollBar>
#include <QEvent>

QSize UnboundedScrollArea::sizeHint() const
{
    const int doubleFrameWidth = 2 * frameWidth();
    QSize size(doubleFrameWidth, doubleFrameWidth);

    if (widget())
        size += widgetResizable() ? widget()->sizeHint() : widget()->size();

    if (verticalScrollBar()->isVisible())
        size.setWidth(size.width() + verticalScrollBar()->sizeHint().width());
    if (horizontalScrollBar()->isVisible())
        size.setHeight(size.height() + horizontalScrollBar()->sizeHint().height());

    return size;
}

bool UnboundedScrollArea::event(QEvent *event) {
    if (event->type() == QEvent::LayoutRequest && widget()) {
        if (widgetResizable())
            updateGeometry();
    }
    return QScrollArea::event(event);
}
