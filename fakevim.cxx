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
		cursor_x = std::min(cell_x, widget->textAtLine(cursor_y).size() - ((editing_mode == EDITING_MODE_COMMAND) ? 1 : 0));
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
			auto s = widget->textAtLine(cursor_y);
			if (c == '\n')
			{
				widget->setLineAtIndex(s.left(cursor_x), cursor_y);
				widget->insertLineAtIndex(s.right(s.size() - cursor_x), cursor_y + 1);
				executeCommandString(QChar(DOWN_ARROW_CODE));
			}
			else
			{
				widget->setLineAtIndex(s.insert(cursor_x, c), cursor_y);
				setCursor(cursor_x + 1, cursor_y);
				virtual_cursor_x = cursor_x;
			}
		}
			break;
		case EDITING_MODE_COMMAND:
			switch (c.cell())
			{
			case 'i':
				editing_mode = EDITING_MODE_INSERT;
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'O':
				editing_mode = EDITING_MODE_INSERT;
				widget->insertLineAtIndex("\n", cursor_y);
				setCursor(0, cursor_y);
				virtual_cursor_x = cursor_x;
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'o':
				editing_mode = EDITING_MODE_INSERT;
				widget->insertLineAtIndex("\n", cursor_y + 1);
				setCursor(0, cursor_y + 1);
				virtual_cursor_x = cursor_x;
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'a':
				editing_mode = EDITING_MODE_INSERT;
				setCursor(cursor_x + 1, cursor_y);
				virtual_cursor_x = cursor_x;
				widget->setCursorType(VGAWidget::CURSOR_TYPE_INSERT);
				break;
			case 'x':
			{
				auto s = widget->textAtLine(cursor_y);
				if (cursor_x < s.size())
					s = s.remove(cursor_x, 1);
				widget->setLineAtIndex(s, cursor_y);
				setCursor(cursor_x, cursor_y);
				virtual_cursor_x = cursor_x;
			}
				break;
			case 'j':
handle_down_arrow:
				setCursor(virtual_cursor_x, cursor_y + 1);
				break;
			case 'k':
handle_up_arrow:
				setCursor(virtual_cursor_x, cursor_y - 1);
				break;
			case 'h':
handle_left_arrow:
				setCursor(cursor_x - 1, cursor_y), virtual_cursor_x = cursor_x;
				break;
			case 'l':
handle_right_arrow:
				setCursor(cursor_x + 1, cursor_y), virtual_cursor_x = cursor_x;
				break;
			}
			break;
		}
	}
}
