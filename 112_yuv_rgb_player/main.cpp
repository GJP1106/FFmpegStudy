#include "yuvrgbplayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	yuvRgbPlayer w;
	w.show();
	return a.exec();
}
