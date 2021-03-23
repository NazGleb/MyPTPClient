#include "MyClient.h"
#include <QtWidgets>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MyClient test;
	test.setFixedSize(400, 400);
	test.show();
	return app.exec();
}