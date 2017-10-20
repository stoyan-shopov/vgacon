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
	virtual void clear(void) { textLines.clear(); update(); }
	void setText(const QByteArray & data);
	void addLine(const QByteArray & line) { textLines += line; greatest_text_line_length = std::max(greatest_text_line_length, line.length()); update(); }
	void setLastLine(const QByteArray & line) { if (textLines.empty()) textLines += line; else * (textLines.end() - 1) = line; greatest_text_line_length = std::max(greatest_text_line_length, line.length()); update(); }
	void setScale(int scale) { this->scale = scale ? scale : 1; update(); }
	void appendText(const QByteArray & text)
	{ if (text.isEmpty()) return; auto lines = text.split('\n'); auto l = lines.constBegin(); if (textLines.isEmpty()) addLine(""); do { textLines.last() += * l; if (++ l != lines.constEnd()) addLine(""); } while (l != lines.constEnd()); update(); }
public:
	enum CURSOR_TYPE
	{
		CURSOR_TYPE_INSERT,
		CURSOR_TYPE_NAVIGATE,
	};

	void setLineAtIndex(const QByteArray & text, int line_number)
	{ /*! \todo - why doesn't this work ???
		this->text.at(line_number) = text; */
		if (line_number >= textLines.size())
			return;
		textLines[line_number] = text;
		greatest_text_line_length = std::max(greatest_text_line_length, text.length());
		update();
	}
	const QByteArray & lineAtIndex(int index) { return textLines.at(index); }
	void setCursorXY(int x, int y) { cursor_x = x, cursor_y = y; update(); }
	void clampCursorX(void) { if (cursor_x < 0) cursor_x = 0, update(); if (cursor_y < textLines.size()) { auto i = textLines.at(cursor_y).size(); if (cursor_x > i) cursor_x = i, update(); } }
	int lineCount(void) { return textLines.size(); }
	void setCursorType(enum CURSOR_TYPE cursor_type) { this->cursor_type = cursor_type; update(); }
	void insertLineAtIndex(const QByteArray & text, int index) { textLines.insert(index, text); update(); }
	void removeLineAtIndex(int index) { textLines.removeAt(index); update(); }
signals:
	void cellSelected(int cell_x, int cell_y);
protected:
	int scaledFontHeight(void) const { return scale * vga_font.height(); }
	int scaledFontWidth(void) const { return scale * vga_font.width(); }
	int cursor_x = -1, cursor_y = -1;
private:
	bool draw_margin = false;
	enum CURSOR_TYPE cursor_type = CURSOR_TYPE_NAVIGATE;
	int scale = 1;
	int sel_x = -1, sel_y = -1;
	int mouse_press_x, mouse_press_y;
	QList<QByteArray> textLines;
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
	virtual void paintEvent(QPaintEvent * event) override;
	void mousePressEvent(QMouseEvent * event) override;
};

#endif // VGAWIDGET_HXX
