#include "sdlqtrgb.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	sdlqtrgb w;
	w.show();
	return a.exec();
}
