#include <QDebug>
#include "fakevim.hxx"

FakeVim::FakeVim(QObject *parent, VGAWidget *editor_widget) : QObject(parent)
{
	widget = editor_widget;
	widget->setCursorXY(cursor_x, cursor_y);
	widget->setCursorType(widget->CURSOR_TYPE_NAVIGATE);
	connect(widget, SIGNAL(cellSelected(int,int)), this, SLOT(cellSelected(int,int)));
}

void FakeVim::setCursor(int cell_x, int cell_y)
{
	cursor_x = cursor_y = -1;
	cursor_y = std::max(std::min(cell_y, widget->lineCount() - 1), 0);
	if (cursor_y < widget->lineCount())
		cursor_x = std::min(cell_x, widget->lineAtIndex(cursor_y).size() - ((editing_mode == EDITING_MODE_COMMAND) ? 1 : 0));
	cursor_x = std::max(cursor_x, 0);
	widget->setCursorXY(cursor_x, cursor_y);
}

void FakeVim::cellSelected(int cell_x, int cell_y)
{
	setCursor(cell_x, cell_y);
	virtual_cursor_x = cursor_x;
}

void FakeVim::executeCommandString(const QString &commands)
{
	for (auto c : commands)
	{
		switch (c.cell()) {
			case DOWN_ARROW_CODE: goto handle_down_arrow;
			case UP_ARROW_CODE: goto handle_up_arrow;
			case LEFT_ARROW_CODE: goto handle_left_arrow;
			case RIGHT_ARROW_CODE: goto handle_right_arrow;
			case ESCAPE_CODE: if (editing_mode == EDITING_MODE_INSERT)
			{
				editing_mode = EDITING_MODE_COMMAND;
				widget->setCursorType(widget->CURSOR_TYPE_NAVIGATE);
				executeCommandString(QChar(LEFT_ARROW_CODE));
			}
			break;
		}
		switch (editing_mode)
		{
		case EDITING_MODE_INSERT:
		{
			auto s = widget->lineAtIndex(cursor_y);
			switch (c.cell())
			{
			case '\n':
				widget->setLineAtIndex(s.left(cursor_x), cursor_y);
				widget->insertLineAtIndex(s.right(s.size() - cursor_x), cursor_y + 1);
				executeCommandString(QString(DOWN_ARROW_CODE) + "\x1bI");
				break;
			case '\b':
				if (cursor_x)
					executeCommandString(QString("\x1bxa") + LEFT_ARROW_CODE);
				break;
			case '\t':
			{
				QString tab_spaces(TAB_SPACES - (cursor_x & 7), ' ');
				executeCommandString(tab_spaces);
				break;
			}
			default:
				widget->setLineAtIndex(s.insert(cursor_x, c), cursor_y);
				setVCursor(cursor_x + 1, cursor_y);
				break;
			}
		}
			break;
		case EDITING_MODE_COMMAND:
			switch (c.cell())
			{
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				/* special case */
				if (repeat_count == -1 && c == '0')
					setVCursor(0, cursor_y);
				else
				{
					if (repeat_count == -1)
						repeat_count = 0;
					repeat_count *= 10;
					repeat_count += c.cell() - '0';
				}
				break;
			case 'w':
			{
				auto s = widget->lineAtIndex(cursor_y);
				int i = cursor_x;
				while (i < s.size() && !QChar(s.at(i)).isSpace()) ++ i;
				while (i < s.size() && QChar(s.at(i)).isSpace()) ++ i;
				if (i != s.size())
					setVCursor(i, cursor_y);
				else
				{
					setVCursor(0, cursor_y + 1);
					s = widget->lineAtIndex(cursor_y);
					while (i < s.size() && QChar(s.at(i)).isSpace()) ++ i;
					if (i != s.size())
						setVCursor(i, cursor_y);
				}
			}
				break;
			case 'J':
				if (cursor_y < widget->lineCount() - 1)
				{
					//widget->setLineAtIndex(widget->lineAtIndex(cursor_y) + " "); + widget->lineAtIndex(cursor_y + 1), cursor_y);
					widget->setLineAtIndex(widget->lineAtIndex(cursor_y) + " ", cursor_y);
					executeCommandString("\x1b$");
					widget->setLineAtIndex(widget->lineAtIndex(cursor_y) + widget->lineAtIndex(cursor_y + 1), cursor_y);
					widget->removeLineAtIndex(cursor_y + 1);
				}
				break;
			case 'i':
				editing_mode = EDITING_MODE_INSERT;
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'I':
				editing_mode = EDITING_MODE_INSERT;
				setVCursor(0, cursor_y);
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'O':
				editing_mode = EDITING_MODE_INSERT;
				widget->insertLineAtIndex("", cursor_y);
				setVCursor(0, cursor_y);
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'o':
				editing_mode = EDITING_MODE_INSERT;
				widget->insertLineAtIndex("", cursor_y + 1);
				setVCursor(0, cursor_y + 1);
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'A':
				executeCommandString("$a");
				break;
			case 'a':
				editing_mode = EDITING_MODE_INSERT;
				setVCursor(cursor_x + 1, cursor_y);
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'x':
			{
				auto s = widget->lineAtIndex(cursor_y);
				if (cursor_x < s.size())
					s = s.remove(cursor_x, getRepeatCount());
				widget->setLineAtIndex(s, cursor_y);
				setVCursor(cursor_x, cursor_y);
			}
				break;
			case 'j':
handle_down_arrow:
				setCursor(virtual_cursor_x, cursor_y + getRepeatCount());
				break;
			case 'k':
handle_up_arrow:
				setCursor(virtual_cursor_x, cursor_y - getRepeatCount());
				break;
			case '\b':
			case 'h':
handle_left_arrow:
				setVCursor(cursor_x - getRepeatCount(), cursor_y);
				break;
			case 'l':
handle_right_arrow:
				setVCursor(cursor_x + getRepeatCount(), cursor_y);
				break;
			case '$':
				setVCursor(widget->lineAtIndex(cursor_y).size(), cursor_y);
				break;
			}
			break;
		}
	}
}
