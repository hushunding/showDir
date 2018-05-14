#include "showdir.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	ShowDir w;
	w.show();
	return a.exec();
}
