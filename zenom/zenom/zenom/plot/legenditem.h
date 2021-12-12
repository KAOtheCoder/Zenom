#ifndef LEGENDITEM_H
#define LEGENDITEM_H

#include <QWidget>
#include "ui_legenditem.h"

namespace Ui {
class LegendItem;
}

class LegendItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit LegendItem(QWidget *parent = 0);
    ~LegendItem();

    QString name() const { return ui->curveName->text(); }
    void setName( const QString& pName );

    void setColor( const QColor& pColor );

    void setSize( const int );

signals:
    void colorChanged( const QColor& );
    void removeRequested();
    void widthChanged( int );
    
private:
    Ui::LegendItem *ui;
};

#endif // LEGENDITEM_H
