#include <QDebug>
#include "vgabutton.hxx"

VGAButton::VGAButton(QWidget *parent) : VGAWidget(parent)
{
	setText("123");
	setMinimumWidth(scaledFontWidth() * (lineAtIndex(0).size() + 2));
	setMinimumHeight(scaledFontHeight() * 2);
	setHorizontalAlignment(CENTER);
	setVerticalAlignment(CENTER);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
}

void VGAButton::mouseMoveEvent(QMouseEvent *event)
{
	setBackgroundColor(rect().contains(event->pos()) ? VGAFont::BLUE : VGAFont::BLACK);
}

void VGAButton::mousePressEvent(QMouseEvent *event)
{
	setBackgroundColor(VGAFont::BLUE);
}

void VGAButton::mouseReleaseEvent(QMouseEvent *event)
{
	setBackgroundColor(VGAFont::BLACK);
	if (rect().contains(event->pos()))
		emit clicked();
}
