#include <algorithm>
#include <iostream>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>
#include "vgawidget.hxx"

const QRgb VGAFont::color_codes[COLOR_CODES_COUNT] =
{
/*
Color palette:

CGA	EGA	VGA	RGB	Web	Example
0x0	0x0	0,0,0	0,0,0	#000000	black
0x1	0x1	0,0,42	0,0,170	#0000aa	blue
0x2	0x2	00,42,00	0,170,0	#00aa00	green
0x3	0x3	00,42,42	0,170,170	#00aaaa	cyan
0x4	0x4	42,00,00	170,0,0	#aa0000	red
0x5	0x5	42,00,42	170,0,170	#aa00aa	magenta
0x6	0x14	42,21,00	170,85,0	#aa5500	brown
0x7	0x7	42,42,42	170,170,170	#aaaaaa	gray
0x8	0x38	21,21,21	85,85,85	#555555	dark gray
0x9	0x39	21,21,63	85,85,255	#5555ff	bright blue
0xA	0x3A	21,63,21	85,255,85	#55ff55	bright green
0xB	0x3B	21,63,63	85,255,255	#55ffff	bright cyan
0xC	0x3C	63,21,21	255,85,85	#ff5555	bright red
0xD	0X3D	63,21,63	255,85,255	#ff55ff	bright magenta
0xE	0x3E	63,63,21	255,255,85	#ffff55	yellow
0xF	0x3F	63,63,63	255,255,255	#ffffff	white
*/
	[BLACK]		=	0xff000000,
	[BLUE]		=	0xff0000aa,
	[GREEN]		=	0xff00aa00,
	[CYAN]		=	0xff00aaaa,
	[RED]		=	0xffaa0000,
	[MAGENTA]	=	0xffaa00aa,
	[BROWN]		=	0xffaa5500,
	[GRAY]		=	0xffaaaaaa,
	[DARK_GRAY]	=	0xff555555,
	[BRIGHT_BLUE]	=	0xff5555ff,
	[BRIGHT_GREEN]	=	0xff55ff55,
	[BRIGHT_CYAN]	=	0xff55ffff,
	[BRIGHT_RED]	=	0xffff5555,
	[BRIGHT_MAGENTA]=	0xffff55ff,
	[YELLOW]	=	0xffffff55,
	[WHITE]		=	0xffffffff,
};
VGAFont vga_font;

VGAWidget::VGAWidget(QWidget *parent)
	: QWidget(parent)
{
}

VGAWidget::~VGAWidget()
{

}

QImage VGAFont::imageForCharacter(uint8_t character_code, int foreground_color_code, int background_color_code)
{
	/*! \todo	for some reason, initializing the font image with an in-class initializer does not work on android */
	if (font_image.isNull())
		font_image = QImage(":/vga-font-8x12-cp1251.png");
#if 0
	 /*! \todo	edit the font bitmap appropriately instead of applying fixes here */
	if (character_code == ' ')
		character_code = 255;
	else
		character_code --;
#endif
QImage i(font_image.copy
	 ((character_code % FONT_BITMAP_CHARACTER_COLUMNS) * VGA_FONT_WIDTH + FONT_X_OFFSET,
		((character_code / FONT_BITMAP_CHARACTER_COLUMNS) + /* offset - maybe edit the bitmap */ 0) * VGA_FONT_HEIGHT + FONT_Y_OFFSET,
		VGA_FONT_WIDTH, VGA_FONT_HEIGHT)
	 );
	i.setColor(FOREGROUND_COLOR_INDEX, color_codes[foreground_color_code]);
	i.setColor(BACKGROUND_COLOR_INDEX, color_codes[background_color_code]);
	return i;
}

void VGAWidget::paintEvent(QPaintEvent *event)
{
const int rows = height() / scaled_height(), columns = width() / scaled_width();
QPixmap px(columns * scaled_width(), rows * scaled_height());
uint8_t text_data[rows * columns], * t(text_data);
	memset(text_data, ' ', sizeof text_data);
	auto l = (text.size() > rows) ? text.cend() - rows : text.cbegin();
	while (l != text.cend())
		memcpy(t, l->toLocal8Bit().constData(), std::min(columns, l->size())), t += columns, ++ l;

	QPainter p(& px);
	p.fillRect(0, 0, px.width(), px.height(), Qt::black);
	t = text_data;
	for (int y = 0; y < rows; y ++)
		for (int x = 0; x < columns; x ++)
		{
			QImage i, si;
			p.drawImage(x * scaled_width(), y * scaled_height(),
				    si = (i = vga_font.imageForCharacter(* t ++, (x == sel_x && y == sel_y) ? VGAFont::YELLOW : VGAFont::CYAN, VGAFont::BLACK))
							.scaled(scaled_width(), scaled_height()));
		}
	QPainter painter(this);
	painter.drawPixmap(0, 0, px);
}

void VGAWidget::mousePressEvent(QMouseEvent *event)
{
	sel_x = event->pos().x() / scaled_width();
	sel_y = event->pos().y() / scaled_height();
	update();
}
