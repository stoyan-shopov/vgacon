#ifndef TOUCHKEYBOARD_HXX
#define TOUCHKEYBOARD_HXX

#include <stdint.h>
#include <QWidget>
#include "vgawidget.hxx"

class TouchKeyboard : public QWidget
{
	Q_OBJECT
public:
	explicit TouchKeyboard(QWidget *parent = nullptr);

private:
	QPixmap pixmap;
	QPixmap pixmapForCharacter(uint8_t character_code);
	QVector<QPair<uint8_t, QRect> > keypads;
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent * event) { *(int*)0=0; }
signals:
	void keyPressed(uint8_t character_code);

public slots:
};

#endif // TOUCHKEYBOARD_HXX
