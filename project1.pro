
TEMPLATE = app
QT = gui core opengl
CONFIG += qt debug warn_on console opengl
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
LIBS += -lGLEW
DEPENDPATH += . src/main src/re167
INCLUDEPATH += . src/main src/re167

SOURCES += src/re167/BasicMath.cpp \
 src/re167/GLRenderContext.cpp \
 src/re167/GLWidget.cpp \
 src/main/main.cpp \
 src/main/MainWidget.cpp \
 src/main/MainWindow.cpp \
 src/re167/Matrix4.cpp \
 src/re167/RenderContext.cpp \
 src/main/RenderWidget0.cpp \
 src/re167/SceneManager.cpp \
 src/re167/VertexData.cpp \
 src/re167/VertexDeclaration.cpp \
 src/main/ColorFactory.cpp \
 src/main/GeometryFactory.cpp \
 src/re167/SWWidget.cpp \
 src/re167/Vector3.cpp \
 src/re167/Vector4.cpp \
 src/re167/Frustum.cpp \
 src/re167/Camera.cpp \
 src/re167/PGMReader.cpp \
 src/re167/ObjReader.cpp \
 src/re167/SWRenderWidget.cpp \
 src/re167/SWZBuffer.cpp \
 src/re167/SWRenderContext.cpp \
 src/re167/Texture.cpp \
 src/re167/Light.cpp \
 src/re167/Material.cpp \
 src/re167/Shader.cpp \
 src/re167/FirstPersonCamera.cpp


 

HEADERS += src/re167/BasicMath.h \
 src/re167/Camera.h \
 src/re167/Frustum.h \
 src/re167/GLRenderContext.h \
 src/re167/GLRenderWidget.h \
 src/re167/GLWidget.h \
 src/main/MainWidget.h \
 src/main/MainWindow.h \
 src/re167/Matrix4.h \
 src/re167/Node.h \
 src/re167/Object.h \
 src/re167/RenderContext.h \
 src/re167/RenderWidget.h \
 src/main/RenderWidget0.h \
 src/re167/SceneManager.h \
 src/re167/Singleton.h \
 src/re167/Vector3.h \
 src/re167/Vector4.h \
 src/re167/VertexData.h \
 src/re167/VertexDeclaration.h \
 src/re167/RE167_global.h \
 src/main/ColorFactory.h \
 src/main/GeometryFactory.h \
 src/re167/SWWidget.h \
 src/re167/Vector3.h \
 src/re167/Vector4.h \
 src/re167/Frustum.h \
 src/re167/Camera.h \
 src/re167/PGMReader.h \
 src/re167/ObjReader.h \
 src/re167/SWRenderWidget.h \
 src/re167/SWZBuffer.h \
 src/re167/SWRenderContext.h \
 src/re167/Light.h \
 src/re167/Texture.h \
 src/re167/Material.h \
 src/re167/Materials.h \
 src/re167/Shader.h \
 src/re167/FirstPersonCamera.h
	
#FORMS += ui/basicapp.ui
