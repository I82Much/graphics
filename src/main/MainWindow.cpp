#include <QtGui>
#include "MainWindow.h"

const int MainWindow::MAX_WIDTH = 1024;
const int MainWindow::MAX_HEIGHT = 768;


MainWindow::MainWindow()
{
	createMenus();
	setWindowTitle("CSCI330 Rendering Engine");

	// the menu bar (21 pixels) is part of the widget size...
    resize(512,512+21);

    // Ensure that the window never gets bigger than this amount
    setMaximumSize(MAX_WIDTH, MAX_HEIGHT);

	renderWidget = new RE167::RenderWidget0();
	setCentralWidget(renderWidget);
}

void MainWindow::createMenus()
{
	exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	startAct = new QAction(tr("St&art animation"), this);
    startAct->setShortcut(tr("Ctrl+a"));
    connect(startAct, SIGNAL(triggered()), this, SLOT(startAnimation()));
	
	stopAct = new QAction(tr("St&op animation"), this);
    stopAct->setShortcut(tr("Ctrl+o"));
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stopAnimation()));

	wireframeAct = new QAction(tr("&Wireframe"), this);
	wireframeAct->setShortcut(tr("Ctrl+w"));
	connect(wireframeAct, SIGNAL(triggered()), this, SLOT(toggleWireframe()));


    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAct);

    animationMenu = menuBar()->addMenu(tr("&Animation"));
    animationMenu->addAction(startAct);
    animationMenu->addAction(stopAct);
    animationMenu->addAction(wireframeAct);
}

void MainWindow::startAnimation()
{
	renderWidget->startAnimation();
}

void MainWindow::stopAnimation()
{
	renderWidget->stopAnimation();
}

void MainWindow::toggleWireframe()
{
	renderWidget->toggleWireframe();
}
