#include <QApplication>
#include "MainWindow.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "GeometryFactory.h"
int main(int argc, char *argv[])
{
//	Vector3::runTestSuite();
//	Matrix4::runTestSuite();
    GeometryFactory::runTestSuite();
	QApplication app(argc, argv);
    MainWindow window;
    window.show();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
