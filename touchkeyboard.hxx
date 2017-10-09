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
	enum
	{
		KEYPAD_WIDTH_IN_PIXELS			=	720,
		KEYPAD_ROW_GLYPH_COUNT_OPTIMIZED	=	11,
		KEYPAD_GLYPH_SCALE_X			=	4,
		KEYPAD_GLYPH_SCALE_Y			=	4,
		KEYPAD_GLYPH_TOTAL_WIDTH		=	KEYPAD_WIDTH_IN_PIXELS / KEYPAD_ROW_GLYPH_COUNT_OPTIMIZED,
		KEYPAD_GLYPH_HEIGHT_MARGIN		=	6,
		KEYPAD_BORDER_PIXELS			=	1,
	};
	QPixmap pixmap;
	QPixmap pixmapForCharacter(uint8_t character_code);
	QVector<QPair<uint8_t, QRect> > keypads;
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
signals:
	void keyPressed(uint8_t character_code);

public slots:
};

#endif // TOUCHKEYBOARD_HXX
