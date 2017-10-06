#include <QPainter>
#include <QMouseEvent>

#include "touchkeyboard.hxx"

TouchKeyboard::TouchKeyboard(QWidget *parent) : QWidget(parent)
{
auto t = pixmapForCharacter(' ');
static const char * keypad_rows[] =
{
	"`1234567890-=",
	"~!@#$%^&*()_+",
	"qwertyuiop[]{}",
	"asdfghjkl;'\\:\"|",
	"\\|zxcvbnm,./<>?",
	" \010\n|>?",
};
int column, row = 0;

	pixmap = QPixmap(t.width() * 20, t.height() * 7);
QPainter p(& pixmap);
	p.fillRect(pixmap.rect(), Qt::black);

	for (auto chars : keypad_rows)
	{
		column = 0;
		while (* chars)
		{
			p.drawPixmap(++ column * t.width(), 10 + row * t.height(), pixmapForCharacter(* chars));
			keypads << QPair<uint8_t, QRect>(* chars, t.rect().adjusted(column * t.width(), 10 + row * t.height(), column * t.width(), 10 + row * t.height()));
			++ chars;
		}
		row ++;
	}
}

QPixmap TouchKeyboard::pixmapForCharacter(uint8_t character_code)
{
auto px = vga_font.imageForCharacter(character_code, vga_font.CYAN, vga_font.BLACK);
	px = px.scaled(px.width() * 4, px.height() * 4);
QPixmap t(px.width() + 14, px.height() + 6);
QPen pen(Qt::cyan);
QPainter p(& t);

	pen.setWidth(1);
	p.setPen(pen);
	p.fillRect(t.rect(), Qt::black);
	p.drawImage(8, 3, px);
	p.drawRect(t.rect().adjusted(1, 1, -2, -2));
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
