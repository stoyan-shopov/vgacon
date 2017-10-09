#include <algorithm>
#include <iostream>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>
#include "vgawidget.hxx"


VGAWidget::VGAWidget(QWidget *parent)
	: QWidget(parent)
{
}

void VGAWidget::paintEvent(QPaintEvent *event)
{
const int rows = height() / scaled_height(), columns = width() / scaled_width();
QPixmap px(columns * scaled_width(), rows * scaled_height());
uint8_t text_data[rows * columns], * t(text_data);

	memset(text_data, ' ', sizeof text_data);
	auto line = text.cbegin() + viewport_y;
	int row = 0;
	while (line != text.cend() && row < rows)
	{
		memcpy(text_data + row * columns, line->toLocal8Bit().constData() + viewport_x, std::max(std::min(line->length() - viewport_x, columns), 0));
		++ row;
		++ line;
	}
	/*
	auto l = (text.size() > rows) ? text.cend() - rows : text.cbegin();
	while (l != text.cend())
		memcpy(t, l->toLocal8Bit().constData(), std::min(columns, l->size())), t += columns, ++ l;
		*/

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
	sel_x = (mouse_press_x = event->pos().x()) / scaled_width();
	sel_y = (mouse_press_y = event->pos().y()) / scaled_height();
	update();
}

/*
void VGAWidget::mouseReleaseEvent(QMouseEvent *event)
{
		*(int*)0=0;
}
*/

void VGAWidget::mouseMoveEvent(QMouseEvent * event)
{
auto x = viewport_x, y = viewport_y;
int d;
	d = event->pos().x() - mouse_press_x;
	if (std::abs(d) >= scaled_width())
		viewport_x -= d / scaled_width(), viewport_x = std::max(viewport_x, 0), viewport_x = std::min(viewport_x, greatest_text_line_length - 1), mouse_press_x = event->pos().x();
	d = event->pos().y() - mouse_press_y;
	if (std::abs(d) >= scaled_height())
		viewport_y -= d / scaled_height(), viewport_y = std::max(viewport_y, 0), viewport_y = std::min(viewport_y, text.size() - 1), mouse_press_y = event->pos().y();
	update();
}
