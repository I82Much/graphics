#include <QApplication>
#include "MainWindow.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "GeometryFactory.h"
#include "Group.h"

int main(int argc, char *argv[])
{
    
    Group::test();
    /*
	Vector3::runTestSuite();
	
	
	
//	Matrix4::runTestSuite();
    RE167::GeometryFactory::runTestSuite();
	QApplication app(argc, argv);
    MainWindow window;
    window.show();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();*/
}
