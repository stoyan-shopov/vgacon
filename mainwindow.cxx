#include <QDebug>
#include <QKeyEvent>
#include "vgawidget.hxx"
#include "mainwindow.hxx"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//connect(ui->lineEdit, SIGNAL(textChanged(QString)), ui->vgaWidget, SLOT(setLastLine(QString)));
	connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->vgaWidget, SLOT(setScale(int)));
	connect(ui->touchpad, SIGNAL(keyPressed(uint8_t)), this, SLOT(touchpadPressed(uint8_t)));
	vgacon_server.listen("vgacon");
	sforth.start();
	vgacon_server.waitForNewConnection(-1);
	vgacon_socket = vgacon_server.nextPendingConnection();
	vgacon_server.close();
	//connect(vgacon_socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
	vgacon_socket->write("unused . cr\n");
	ui->vgaWidget->installEventFilter(this);
	fakevim = new FakeVim(this, ui->vgaWidget);
	ui->touchpad->hide();

	QProcess sforth_process;
	QDir dir;
	QFile sforth_executable(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + getTargetExecutableFileName());

	dir.mkdir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
	QFile(getBundledExecutableFileName()).copy(sforth_executable.fileName());
	sforth_executable.setPermissions(static_cast<QFile::Permissions>(0x7777));
	sforth_process.start(sforth_executable.fileName());

	sforth_process.write("/data/local/tmp/sf-arm64" "\n");
	sforth_process.write("whoami" "\n");
	sforth_process.write("12 12 * . cr\nbye\n");
	sforth_process.write("exit" "\n");
	sforth_process.waitForFinished();
	ui->vgaWidget->setText(sforth_process.readAll());
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::touchpadPressed(uint8_t character_code)
{
QString s = ui->lineEdit->text();
	switch (character_code)
	{
	case '\n':
		on_lineEdit_returnPressed();
		break;
	case '\b':
		ui->lineEdit->setText(s.left(s.length() - 1));
		break;
	default:
		ui->lineEdit->setText(s + QChar(character_code));
		break;
	}
}

void MainWindow::socketReadyRead()
{
	if (0) while (vgacon_socket->bytesAvailable())
	{
		unsigned char c;
		vgacon_socket->read(reinterpret_cast<char *>(& c), 1);
		qDebug() << c;
	}
	while (vgacon_socket->canReadLine())
		ui->vgaWidget->addLine(vgacon_socket->readLine());
}

void MainWindow::on_lineEdit_returnPressed()
{
	vgacon_socket->write((ui->lineEdit->text() + '\n').toLocal8Bit());
	ui->lineEdit->clear();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	vgacon_socket->close();
	sforth.wait();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == ui->vgaWidget && event->type() == QEvent::KeyPress)
	{
		QKeyEvent * key = reinterpret_cast<QKeyEvent *>(event);
		qDebug() << key->text();
		auto command = key->text();
		/* translate special keys */
		switch (key->key())
		{
			case Qt::Key_Enter: case Qt::Key_Return: command = '\n'; break;
			case Qt::Key_Escape: command = QChar(fakevim->ESCAPE_CODE); break;
			case Qt::Key_Up: command = QChar(fakevim->UP_ARROW_CODE); break;
			case Qt::Key_Down: command = QChar(fakevim->DOWN_ARROW_CODE); break;
			case Qt::Key_Left: command = QChar(fakevim->LEFT_ARROW_CODE); break;
			case Qt::Key_Right: command = QChar(fakevim->RIGHT_ARROW_CODE); break;
		}
		fakevim->executeCommandString(command);
		return true;
	}
	return false;
}
