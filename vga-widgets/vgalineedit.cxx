#include "vgalineedit.hxx"

VGALineEdit::VGALineEdit(QWidget *parent) : VGAWidget(parent)
{
	setMinimumWidth(scaledFontWidth() * 10);
	setMinimumHeight(scaledFontHeight() * 1);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	setText("Lorem ipsum dolor sit amet");
	setCursorXY(text().size(), 0);
	setCursorType(CURSOR_TYPE_INSERT);
	connect(this, VGAWidget::cellSelected, this, [=] (int x, int y) { setCursorXY(x, y); clampCursorX(); });
	show();
}

void VGALineEdit::keyPressEvent(QKeyEvent * event)
{
auto t = text().toLocal8Bit();
bool must_update = false;

	switch (event->key())
	{
		case Qt::Key_Enter: case Qt::Key_Return: emit returnPressed(); return;
		case Qt::Key_Left: if (cursor_x) -- cursor_x; else return;
		if (0)
		case Qt::Key_Right: if (cursor_x != t.size()) ++ cursor_x; else return;
			update(); return;
	}

	if (event->text() == "\b")
	{
		if (cursor_x)
			t.remove(cursor_x - 1, 1), -- cursor_x, must_update = true;
	}
	else
		t.insert(cursor_x, event->text()), cursor_x += event->text().size(), must_update = true;
	if (must_update)
		setText(t);
}
