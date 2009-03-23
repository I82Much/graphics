#ifndef __MainWindow_h__
#define __MainWindow_h__

#include <QMainWindow>
#include "RenderWidget0.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
    static const int MAX_WIDTH;
    static const int MAX_HEIGHT;

private slots:
	void startAnimation();
	void stopAnimation();
	void toggleWireframe();

private:
	void createMenus();

	QMenu *fileMenu;
	QMenu *animationMenu;
	QAction *exitAct;
	QAction *startAct;
	QAction *stopAct;
	QAction *wireframeAct;

	RenderWidget0 *renderWidget;
};

#endif
