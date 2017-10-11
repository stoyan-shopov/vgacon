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

void VGAWidget::setText(const QByteArray &data)
{
	auto lines = data.split('\n');
	text.clear();
	greatest_text_line_length = 0;
	for (auto l : lines)
		text << l, greatest_text_line_length = std::max(greatest_text_line_length, l.size());
	update();
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
	QPen pen(Qt::red);
	p.setPen(pen);
	p.drawRect((cursor_x - viewport_x) * scaled_width(), (cursor_y - viewport_y) * scaled_height(), scaled_width(), scaled_height());
	pen = QPen(Qt::yellow);
	p.setPen(pen);
	auto margin_x = (greatest_text_line_length - viewport_x) * scaled_width(), margin_y = (text.size() - viewport_y) * scaled_height();
	p.drawLine(0, margin_y, margin_x, margin_y);
	p.drawLine(margin_x, 0, margin_x, margin_y);
	QPainter painter(this);
	painter.drawPixmap(0, 0, px);
}

void VGAWidget::mousePressEvent(QMouseEvent *event)
{
	sel_x = (mouse_press_x = event->pos().x()) / scaled_width();
	sel_y = (mouse_press_y = event->pos().y()) / scaled_height();
	emit cellSelected(sel_x, sel_y);
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
