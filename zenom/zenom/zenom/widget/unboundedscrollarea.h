#ifndef UNBOUNDEDSCROLLAREA_H
#define UNBOUNDEDSCROLLAREA_H

#include <QScrollArea>

class UnboundedScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    UnboundedScrollArea(QWidget* parent = nullptr) : QScrollArea(parent) {}

    virtual QSize sizeHint() const override;

protected:
    bool event(QEvent* event) override;
};

#endif // UNBOUNDEDSCROLLAREA_H
