#include <QPainter>
#include <QMouseEvent>

#include "touchkeyboard.hxx"

TouchKeyboard::TouchKeyboard(QWidget *parent) : QWidget(parent)
{
auto t = pixmapForCharacter(' ');
static const char * keypad_rows[] =
{
	"!@#$%^&*()",
	"1234567890",
	"[]{};:'\"|.",
	"qwertyuiop",
	"asdfghjkl",
	"zxcvbnm",
	" \010\n|>?",
};
int column, row = 0;
QPen clip_pen(Qt::yellow);

	pixmap = QPixmap(t.width() * KEYPAD_ROW_GLYPH_COUNT_OPTIMIZED, t.height() * sizeof keypad_rows / sizeof * keypad_rows);
QPainter p(& pixmap);

	p.setPen(clip_pen);
	p.fillRect(pixmap.rect(), Qt::black);
	p.drawRect(pixmap.rect());

	for (auto chars : keypad_rows)
	{
		column = 0;
		while (* chars)
		{
			p.drawPixmap(column * t.width(), row * t.height(), pixmapForCharacter(* chars));
			keypads << QPair<uint8_t, QRect>(* chars, t.rect().adjusted(column * t.width(), row * t.height(), column * t.width(), row * t.height()));
			++ column;
			++ chars;
		}
		row ++;
	}
	auto bottom_right_row = row - 1, bottom_right_column = column - 1;
	/* create special keypad inputs - the 'space bar', 'enter', and 'backspace' */
	/* create 'enter' keypad input area */
	keypads << QPair<uint8_t, QRect>('\n', t.rect().adjusted(column * t.width(), row * t.height(), column * t.width(), row * t.height()));
	QPointF return_key_border[] =
	{
		{ pixmap.width() - 1, pixmap.height() - 1, },
		{ pixmap.width() - 2 * t.width() - 1, pixmap.height() - 1, },
		{ pixmap.width() - 2 * t.width() - 1, pixmap.height() - 1 * t.height() - 1, },
		{ pixmap.width() - 1 * t.width() - 1, pixmap.height() - 1 * t.height() - 1, },
		{ pixmap.width() - 1 * t.width() - 1, pixmap.height() - 2 * t.height() - 1, },
		{ pixmap.width() - 1, pixmap.height() - 2 * t.height() - 1, },
	};
	p.drawPolygon(return_key_border, sizeof return_key_border / sizeof * return_key_border);
	setMinimumSize(KEYPAD_WIDTH_IN_PIXELS, pixmap.height());
}

QPixmap TouchKeyboard::pixmapForCharacter(uint8_t character_code)
{
QPen pen(vga_font.vgaRGBColor(vga_font.BRIGHT_CYAN));
QBrush brush(vga_font.vgaRGBColor(vga_font.DARK_GRAY));
auto px = vga_font.imageForCharacter(character_code, vga_font.WHITE, vga_font.DARK_GRAY);
	px = px.scaled(px.width() * KEYPAD_GLYPH_SCALE_X, px.height() * KEYPAD_GLYPH_SCALE_Y);
QPixmap t(KEYPAD_GLYPH_TOTAL_WIDTH, px.height() + KEYPAD_GLYPH_HEIGHT_MARGIN);
QPainter p(& t);
auto mitre_size = std::max((t.width() >> 3) - 2 * KEYPAD_BORDER_PIXELS, (t.height() >> 3) - KEYPAD_BORDER_PIXELS);
QPointF keypad_border[] = {
	{ mitre_size, KEYPAD_BORDER_PIXELS, },
	{ t.width() - mitre_size - KEYPAD_BORDER_PIXELS - 1, KEYPAD_BORDER_PIXELS, },
	{ t.width() - KEYPAD_BORDER_PIXELS - 1, mitre_size, },
	{ t.width() - KEYPAD_BORDER_PIXELS - 1, t.height() - mitre_size - KEYPAD_BORDER_PIXELS - 1, },
	{ t.width() - mitre_size - KEYPAD_BORDER_PIXELS - 1, t.height() - KEYPAD_BORDER_PIXELS - 1, },
	{ mitre_size, t.height() - KEYPAD_BORDER_PIXELS - 1, },
	{ KEYPAD_BORDER_PIXELS, t.height() - mitre_size - 1, },
	{ KEYPAD_BORDER_PIXELS, mitre_size, },
};

	pen.setWidth(1);
	p.setPen(pen);
	p.setBrush(brush);
	p.fillRect(t.rect(), Qt::black);
	p.drawPolygon(keypad_border, sizeof keypad_border / sizeof * keypad_border);
	p.drawImage(KEYPAD_GLYPH_TOTAL_WIDTH >> 2, KEYPAD_GLYPH_HEIGHT_MARGIN >> 1, px);
	return t;
}

void TouchKeyboard::paintEvent(QPaintEvent *event)
{
QPainter p(this);
p.drawPixmap(0, 0, pixmap);
}

void TouchKeyboard::mousePressEvent(QMouseEvent *event)
{
	for (auto i : keypads)
		if (i.second.contains(event->pos()))
			emit keyPressed(i.first);
}
