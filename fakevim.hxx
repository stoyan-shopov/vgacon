#ifndef FAKEVIM_HXX
#define FAKEVIM_HXX

#include <QObject>
#include <vgawidget.hxx>

class FakeVim : public QObject
{
	Q_OBJECT

private:
	VGAWidget	* widget;
	int cursor_x = 0, cursor_y = 0;
	int virtual_cursor_x = 0;
	void setCursor(int cell_x, int cell_y);
	enum EDITING_MODE
	{
		EDITING_MODE_COMMAND,
		EDITING_MODE_INSERT,
	}
	editing_mode = EDITING_MODE_COMMAND;
public:
	enum
	{
		/*! \note	these are pretty arbitrary */
		UP_ARROW_CODE		= 255 - 0,
		DOWN_ARROW_CODE		= 255 - 1,
		LEFT_ARROW_CODE		= 255 - 2,
		RIGHT_ARROW_CODE	= 255 - 3,
		ESCAPE_CODE		= 27,
	};
	FakeVim(QObject * parent, VGAWidget * editor_widget);
	void executeCommandString(const QString & commands);
private slots:
	void cellSelected(int cell_x, int cell_y);
};

#endif // FAKEVIM_HXX
