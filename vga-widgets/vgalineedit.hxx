#ifndef VGALINEEDIT_HXX
#define VGALINEEDIT_HXX

#include "vgawidget.hxx"

class VGALineEdit : public VGAWidget
{
	Q_OBJECT
public:
	explicit VGALineEdit(QWidget *parent = nullptr);
	virtual QSize sizeHint() { *(int*)0=0;return QSize(200, scaledFontHeight()); }
signals:
public slots:
};

#endif // VGALINEEDIT_HXX
