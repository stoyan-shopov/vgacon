#include <QDebug>
#include <QKeyEvent>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>

#include "vgawidget.hxx"
#include "mainwindow.hxx"

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

	QDir dir;
	sforth_executable.setFileName(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + getTargetExecutableFileName());

	dir.mkdir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
	QFile(getBundledExecutableFileName()).copy(sforth_executable.fileName());
	sforth_executable.setPermissions(static_cast<QFile::Permissions>(0x7777));
	connect(& sforth_process, & QProcess::readyReadStandardError, [=] { ui->vgaWidget->appendText(sforth_process.readAllStandardError()); });
	connect(& sforth_process, & QProcess::readyReadStandardOutput, [=] { ui->vgaWidget->appendText(sforth_process.readAllStandardOutput()); });
	connect(& sforth_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(sforthProcessFinished(int,QProcess::ExitStatus)));
	startSforthProcess();
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
	while (vgacon_socket->canReadLine())
		ui->vgaWidget->addLine(vgacon_socket->readLine());
}

void MainWindow::on_lineEdit_returnPressed()
{
	sforth_process.write((ui->lineEdit->text() + '\n').toLocal8Bit());
	ui->lineEdit->clear();
}

void MainWindow::sforthProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	qDebug() << sforth_process.state();
	if(exitStatus == QProcess::CrashExit && QMessageBox::critical(this, "sforth process crashed", "the sforth process crashed\ndo you want to restart it?", QMessageBox::Yes, QMessageBox::No)
				== QMessageBox::Yes)
		startSforthProcess();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	sforth_process.kill();
	sforth_process.waitForFinished();
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
