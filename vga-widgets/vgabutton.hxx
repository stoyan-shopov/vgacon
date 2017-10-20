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
protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
};

#endif // VGABUTTON_HXX
