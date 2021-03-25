#include "MyClient.h"
#include <QtWidgets>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MyClient test;
	test.setWindowTitle("My chat v1.6");
	test.setFixedSize(400, 400);
	test.show();
	return app.exec();
}