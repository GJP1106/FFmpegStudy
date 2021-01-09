#include "sdl_qt_play_yuv.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sdl_qt_play_yuv w;
	w.show();
	return a.exec();
}
