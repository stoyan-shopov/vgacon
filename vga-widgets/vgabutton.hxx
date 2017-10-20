#ifndef VGABUTTON_HXX
#define VGABUTTON_HXX

#include <QMouseEvent>
#include "vgawidget.hxx"

class VGAButton : public VGAWidget
{
	Q_OBJECT
public:
	VGAButton(QWidget * parent);
signals:
	void clicked(void);
public slots:
	virtual void setText(const QByteArray & data) override { VGAWidget::setText(data); setMinimumWidth(scaledFontWidth() * (lineAtIndex(0).size() + 2)); }
protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
};

#endif // VGABUTTON_HXX
