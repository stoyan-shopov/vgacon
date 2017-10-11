#include <QDebug>
#include <QKeyEvent>
#include "vgawidget.hxx"
#include "mainwindow.hxx"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//connect(ui->lineEdit, SIGNAL(textChanged(QString)), ui->vgaWidget, SLOT(setLastLine(QString)));
	connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->vgaWidget, SLOT(setScale(int)));
	connect(ui->touchpad, SIGNAL(keyPressed(uint8_t)), this, SLOT(touchpadPressed(uint8_t)));
	connect(ui->vgaWidget, SIGNAL(cellSelected(int,int)), this, SLOT(cellSelected(int,int)));
	vgacon_server.listen("vgacon");
	sforth.start();
	vgacon_server.waitForNewConnection(-1);
	vgacon_socket = vgacon_server.nextPendingConnection();
	vgacon_server.close();
	connect(vgacon_socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
	vgacon_socket->write("unused . cr\n");
	ui->vgaWidget->installEventFilter(this);
	ui->vgaWidget->setCursorXY(cursor_x, cursor_y);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setCursor(int cell_x, int cell_y)
{
	cursor_x = cursor_y = -1;
	cursor_y = std::max(std::min(cell_y, ui->vgaWidget->lineCount() - 1), 0);
	if (cursor_y < ui->vgaWidget->lineCount())
		cursor_x = std::min(cell_x, ui->vgaWidget->textAtLine(cursor_y).size() - 1);
	cursor_x = std::max(cursor_x, 0);
	ui->vgaWidget->setCursorXY(cursor_x, cursor_y);
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
	vgacon_socket->write((ui->lineEdit->text() + '\n').toLocal8Bit());
	ui->lineEdit->clear();
}

void MainWindow::cellSelected(int cell_x, int cell_y)
{
	setCursor(cell_x, cell_y);
	virtual_cursor_x = cursor_x;
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
		if (key->text() == "x")
		{
			auto s = ui->vgaWidget->textAtLine(cursor_y);
			if (cursor_x < s.size())
				s = s.remove(cursor_x, 1);
			ui->vgaWidget->setLineText(cursor_y, s);
		}
		else if (key->text() == "j")
			setCursor(virtual_cursor_x, cursor_y + 1);
		else if (key->text() == "k")
			setCursor(virtual_cursor_x, cursor_y - 1);
		else if (key->text() == "h")
			setCursor(cursor_x - 1, cursor_y), virtual_cursor_x = cursor_x;
		else if (key->text() == "l")
			setCursor(cursor_x + 1, cursor_y), virtual_cursor_x = cursor_x;
	}
	return false;
}
