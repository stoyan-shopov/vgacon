#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QLocalServer>
#include <QLocalSocket>
#include "sforth.hxx"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	enum EDITING_MODE
	{
		EDITING_MODE_COMMAND,
		EDITING_MODE_INSERT,
	}
	editing_mode = EDITING_MODE_COMMAND;

	Ui::MainWindow *ui;
	QLocalServer	vgacon_server;
	QLocalSocket	* vgacon_socket;
	SForth		sforth;
	int cursor_x = 0, cursor_y = 0;
	int virtual_cursor_x = 0;
	void setCursor(int cell_x, int cell_y);
private slots:
	void touchpadPressed(uint8_t character_code);
	void socketReadyRead(void);
	void on_lineEdit_returnPressed();
	void cellSelected(int cell_x, int cell_y);
protected:
	void closeEvent(QCloseEvent * event);
	bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MAINWINDOW_HXX
