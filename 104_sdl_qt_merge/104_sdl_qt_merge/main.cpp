#include "sdl_qt_merge.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sdl_qt_merge w;
	w.show();
	return a.exec();
}
