#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QLocalServer>
#include <QLocalSocket>
#include <QProcess>
#include <QFile>
#include "sforth.hxx"
#include "fakevim.hxx"
#include "ui_mainwindow.h"

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
	FakeVim		* fakevim;
	QProcess	sforth_process;
	QFile sforth_executable;
	void startSforthProcess(void) { sforth_process.start(sforth_executable.fileName()); sforth_process.write("true cr-echo !\n"); }
private slots:
	void touchpadPressed(uint8_t character_code);
	void socketReadyRead(void);
	void on_lineEdit_returnPressed();
	void sforthProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
protected:
	void closeEvent(QCloseEvent * event);
	bool eventFilter(QObject *watched, QEvent *event);
	static const QString getBundledExecutableFileName(void)
	{
		return
#if defined Q_OS_WIN
	":/sf.exe"
#elif defined Q_OS_ANDROID
	":/sf-arm64"
#elif defined Q_OS_LINUX
#error "build the sforth executable for linux, and put it in the resources"
#else
#error "cannot identify build target"
#endif
		;
	}
	static const QString getTargetExecutableFileName(void)
	{
		return
#if defined Q_OS_WIN
	"sf.exe"
#elif defined Q_OS_ANDROID
	"sf-arm64"
#elif defined Q_OS_LINUX
#error "build the sforth executable for linux, and put it in the resources"
#else
#error "cannot identify build target"
#endif
		;
	}
};

#endif // MAINWINDOW_HXX
