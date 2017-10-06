#include <QLocalSocket>
extern "C"
{
#include "engine.h"
}
#include "sforth.hxx"

static QLocalSocket * sforth_socket;

extern "C"
{
int sfgetc(void) { char x; if (!sforth_socket->bytesAvailable()) sforth_socket->waitForReadyRead(-1); sforth_socket->read(& x, sizeof x); return (unsigned) x; }
int sffgetc(cell file_id) { return EOF; }
int sfputc(int c) { sforth_socket->write((char *) & c, 1); sforth_socket->waitForBytesWritten(); return 0; }
int sfsync(void) { sforth_socket->waitForBytesWritten(); return 0; }
cell sfopen(const char * pathname, int flags) { return 0; }
int sfclose(cell file_id) { return EOF; }
int sffseek(cell stream, long offset) { return -1; }
}

void SForth::run()
{
	sforth_socket = new QLocalSocket;
	sforth_socket->connectToServer("vgacon");
	sforth_socket->waitForConnected();
	sf_reset();
	do_quit();
}

SForth::SForth()
{
	//sforth_socket.moveToThread(this);
}
