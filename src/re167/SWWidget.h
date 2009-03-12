#ifndef __SWWidget_h__
#define __SWWidget_h__

#include "RE167_global.h"
#include "SWRenderContext.h"
#include <qtgui>

namespace RE167 {

	class SWRenderWidget;

	/** Internal class. No need to use it directly in applications.
	*/
	class RE167_EXPORT SWWidget : public QLabel
	{
	public:
		SWWidget(SWRenderWidget *parent);
		~SWWidget();

		void updateScene();
		QImage *getImage();

	protected:
		void resizeEvent(QResizeEvent *e);
		void showEvent(QShowEvent *e);

	private:
		QImage *image;
		bool initSceneEventCalled;
	};

}

#endif