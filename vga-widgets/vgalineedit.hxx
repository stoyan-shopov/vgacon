#ifndef VGALINEEDIT_HXX
#define VGALINEEDIT_HXX
#include <QKeyEvent>

#include "vgawidget.hxx"

class VGALineEdit : public VGAWidget
{
	Q_OBJECT
public:
	explicit VGALineEdit(QWidget *parent = nullptr);
	virtual QSize sizeHint() { *(int*)0=0;return QSize(200, scaledFontHeight()); }
	QString text(void) { return lineAtIndex(0); }
signals:
	void returnPressed(void);
public slots:
	void clear(void) { VGAWidget::clear(); setText(""); clampCursorX(); }
protected:
	virtual void keyPressEvent(QKeyEvent * event);
};

#endif // VGALINEEDIT_HXX
