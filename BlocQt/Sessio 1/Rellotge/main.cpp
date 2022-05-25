#include <QApplication>
#include "MyRellotge.h"

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	MyRellotge rellotge;
	rellotge.show();
	return app.exec();
}