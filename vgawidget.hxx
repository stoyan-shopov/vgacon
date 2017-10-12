#ifndef VGAWIDGET_HXX
#define VGAWIDGET_HXX

#include <stdint.h>
#include <QWidget>

#include "vgafont.hxx"


class VGAWidget : public QWidget
{
	Q_OBJECT
public:
	VGAWidget(QWidget *parent = 0);
public slots:
	void clear(void) { text.clear(); repaint(); }
	void setText(const QByteArray & data);
	void addLine(const QString & line) { text += line; greatest_text_line_length = std::max(greatest_text_line_length, line.length()); repaint(); }
	void setLastLine(const QString & line) { if (text.empty()) text += line; else * (text.end() - 1) = line; greatest_text_line_length = std::max(greatest_text_line_length, line.length()); repaint(); }
	void setScale(int scale) { this->scale = scale ? scale : 1; update(); }
public:
	enum CURSOR_TYPE
	{
		CURSOR_TYPE_INSERT,
		CURSOR_TYPE_NAVIGATE,
	};

	void setLineText(int line_number, const QString & text)
	{ /*! \todo - why doesn't this work ???
		this->text.at(line_number) = text; */
		if (line_number >= this->text.size())
			return;
		this->text[line_number] = text;
		greatest_text_line_length = std::max(greatest_text_line_length, text.length());
		repaint();
	}
	const QString & textAtLine(int line_number) { return text.at(line_number); }
	void setCursorXY(int x, int y) { cursor_x = x, cursor_y = y; repaint(); }
	int lineCount(void) { return text.size(); }
	void setCursorType(enum CURSOR_TYPE cursor_type) { this->cursor_type = cursor_type; repaint(); }
	void insertLineAtIndex(const QString text, int index) { this->text.insert(index, text); repaint(); }
	void removeLineAtIndex(int index) { text.removeAt(index); }
signals:
	void cellSelected(int cell_x, int cell_y);
private:
	enum CURSOR_TYPE cursor_type = CURSOR_TYPE_NAVIGATE;
	int scale = 1;
	int cursor_x = -1, cursor_y = -1;
	int scaled_height(void) const { return scale * vga_font.height(); }
	int scaled_width(void) const { return scale * vga_font.width(); }
	int sel_x = -1, sel_y = -1;
	int mouse_press_x, mouse_press_y;
	QStringList text;
	int greatest_text_line_length = 0;
	/* these hold the viewport origin (the top left corner coordinates of the viewport) that this widget currently displays;
	 * the text lines in the 'text' list define a text view of some width and height, the viewport is
	 * a view inside this (usually larger than can be displayed at once) text view, i.e.:
	 *
	 * +-----------------+ ----> overall text view
	 * |                 |
	 * |                 |
	 * |    x-----+      |
	 * |    |     |      |
	 * |    +-----+  -----> viewport; the 'x' at the top left is the viewport origin
	 * |                 |
	 * |                 |
	 * |                 |
	 * |                 |
	 * |                 |
	 * |                 |
	 * +-----------------+
	 *
	 *
	 */
	int viewport_x = 0, viewport_y = 0;

protected:
	void mouseMoveEvent(QMouseEvent * event) override;
	void paintEvent(QPaintEvent * event) override;
	void mousePressEvent(QMouseEvent * event) override;
};

#endif // VGAWIDGET_HXX
