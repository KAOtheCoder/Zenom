#ifndef LINEARGAUGE_H
#define LINEARGAUGE_H

#include "abstractgauge.h"
#include <qwt_thermo.h>
#include <qwt_scale_draw.h>

class LinearGauge : public QwtThermo, public AbstractGauge
{
	Q_OBJECT

public:
	LinearGauge(QWidget *pParent = 0);

	~LinearGauge();

	virtual GaugeType type();
	
    virtual void setValue(double pValue) { QwtThermo::setValue(pValue); }

	virtual void saveSettings( QSettings& pSettings );

	virtual void loadSettings( QSettings& pSettings );

	virtual QWidget* asWidget();

	// Color
	void setTextColor( const QColor& pColor );
	QColor textColor();
	void setBackgroundColor( const QColor& pColor );
	QColor backgroundColor();

	// Why protected ?
	QwtScaleDraw *scaleDraw();
};

#endif // LINEARGAUGE_H
