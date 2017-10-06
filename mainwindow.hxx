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
	Ui::MainWindow *ui;
	QLocalServer	vgacon_server;
	QLocalSocket	* vgacon_socket;
	SForth		sforth;
private slots:
	void touchpadPressed(uint8_t character_code);
	void socketReadyRead(void);
	void on_lineEdit_returnPressed();
};

#endif // MAINWINDOW_HXX
