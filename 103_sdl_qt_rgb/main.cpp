#include "sdl_qt_rgb.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Sdl_Qt_Rgb w;
	w.show();
	return a.exec();
}
