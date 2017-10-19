#include "vgalineedit.hxx"

VGALineEdit::VGALineEdit(QWidget *parent) : VGAWidget(parent)
{
	setMinimumWidth(scaledFontWidth() * 10);
	setMinimumHeight(scaledFontHeight() * 1);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
	setText("lorem ipsum, dolor sit amet");
	show();
}
