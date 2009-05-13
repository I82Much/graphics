#############################################################################
# Makefile for building: bin/project1.app/Contents/MacOS/project1
# Generated by qmake (2.01a) (Qt 4.4.3) on: Wed May 13 18:00:29 2009
# Project:  project1.pro
# Template: app
# Command: /opt/local/bin/qmake-mac -macx -o Makefile project1.pro
#############################################################################

####### Compiler, tools and options

CC            = /usr/bin/gcc-4.0
CXX           = /usr/bin/g++-4.0
DEFINES       = -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W $(DEFINES)
INCPATH       = -I/opt/local/libexec/qt4-mac/mkspecs/macx-g++ -I. -I/opt/local/libexec/qt4-mac/lib/QtCore.framework/Versions/4/Headers -I/opt/local/libexec/qt4-mac/include/QtCore -I/opt/local/libexec/qt4-mac/include/QtCore -I/opt/local/libexec/qt4-mac/lib/QtGui.framework/Versions/4/Headers -I/opt/local/libexec/qt4-mac/include/QtGui -I/opt/local/libexec/qt4-mac/include/QtGui -I/opt/local/libexec/qt4-mac/lib/QtOpenGL.framework/Versions/4/Headers -I/opt/local/libexec/qt4-mac/include/QtOpenGL -I/opt/local/libexec/qt4-mac/include/QtOpenGL -I/opt/local/libexec/qt4-mac/include -I. -Isrc/main -Isrc/re167 -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/AGL.framework/Headers -Ibuild -Ibuild -F/opt/local/libexec/qt4-mac/lib
LINK          = /usr/bin/g++-4.0
LFLAGS        = -headerpad_max_install_names
LIBS          = $(SUBLIBS) -F/opt/local/libexec/qt4-mac/lib -L/opt/local/libexec/qt4-mac/lib -lGLEW -framework QtOpenGL -L/opt/local/lib -L/opt/local/lib/mysql5/mysql -L/opt/local/lib/postgresql83 -L/opt/local/libexec/qt4-mac/lib -F/opt/local/libexec/qt4-mac/lib -framework OpenGL -framework AGL -framework QtGui -framework Carbon -framework AppKit -lpng -framework QtCore -lz -lm -framework ApplicationServices
AR            = ar cq
RANLIB        = ranlib -s
QMAKE         = /opt/local/bin/qmake-mac
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
export MACOSX_DEPLOYMENT_TARGET = 10.3

####### Output directory

OBJECTS_DIR   = build/

####### Files

SOURCES       = src/re167/BasicMath.cpp \
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
		src/re167/FirstPersonCamera.cpp \
		src/re167/scenegraph/Node.cpp \
		src/re167/scenegraph/Group.cpp \
		src/re167/scenegraph/TransformGroup.cpp \
		src/re167/scenegraph/Leaf.cpp \
		src/re167/scenegraph/Shape3D.cpp \
		src/re167/Plane.cpp \
		src/re167/scenegraph/LightNode.cpp \
		src/re167/scenegraph/CameraNode.cpp \
		src/re167/spline/BezierCurve.cpp \
		src/re167/spline/Spline.cpp \
		src/re167/spline/Circle.cpp \
		src/re167/spline/Helix.cpp \
		src/re167/spline/Morpher.cpp \
		src/re167/Basis.cpp \
		src/re167/spline/Square.cpp \
		src/re167/spline/PiecewiseSpline.cpp build/moc_GLWidget.cpp \
		build/moc_MainWindow.cpp
OBJECTS       = build/BasicMath.o \
		build/GLRenderContext.o \
		build/GLWidget.o \
		build/main.o \
		build/MainWidget.o \
		build/MainWindow.o \
		build/Matrix4.o \
		build/RenderContext.o \
		build/RenderWidget0.o \
		build/SceneManager.o \
		build/VertexData.o \
		build/VertexDeclaration.o \
		build/ColorFactory.o \
		build/GeometryFactory.o \
		build/SWWidget.o \
		build/Vector3.o \
		build/Vector4.o \
		build/Frustum.o \
		build/Camera.o \
		build/PGMReader.o \
		build/ObjReader.o \
		build/SWRenderWidget.o \
		build/SWZBuffer.o \
		build/SWRenderContext.o \
		build/Texture.o \
		build/Light.o \
		build/Material.o \
		build/Shader.o \
		build/FirstPersonCamera.o \
		build/Node.o \
		build/Group.o \
		build/TransformGroup.o \
		build/Leaf.o \
		build/Shape3D.o \
		build/Plane.o \
		build/LightNode.o \
		build/CameraNode.o \
		build/BezierCurve.o \
		build/Spline.o \
		build/Circle.o \
		build/Helix.o \
		build/Morpher.o \
		build/Basis.o \
		build/Square.o \
		build/PiecewiseSpline.o \
		build/moc_GLWidget.o \
		build/moc_MainWindow.o
DIST          = /opt/local/libexec/qt4-mac/mkspecs/common/unix.conf \
		/opt/local/libexec/qt4-mac/mkspecs/common/mac.conf \
		/opt/local/libexec/qt4-mac/mkspecs/common/mac-g++.conf \
		/opt/local/libexec/qt4-mac/mkspecs/qconfig.pri \
		/opt/local/libexec/qt4-mac/mkspecs/features/qt_functions.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/qt_config.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/exclusive_builds.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/default_pre.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/default_pre.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/debug.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/default_post.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/default_post.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/objective_c.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/unix/opengl.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/warn_on.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/qt.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/unix/thread.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/moc.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/rez.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/sdk.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/resources.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/uic.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/yacc.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/lex.prf \
		project1.pro
QMAKE_TARGET  = project1
DESTDIR       = bin/
TARGET        = bin/project1.app/Contents/MacOS/project1

####### Custom Compiler Variables
QMAKE_COMP_QMAKE_OBJECTIVE_CFLAGS = -pipe \
		-g \
		-Wall \
		-W


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile bin/project1.app/Contents/PkgInfo bin/project1.app/Contents/Info.plist $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) bin/project1.app/Contents/MacOS/ || $(MKDIR) bin/project1.app/Contents/MacOS/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: project1.pro  /opt/local/libexec/qt4-mac/mkspecs/macx-g++/qmake.conf /opt/local/libexec/qt4-mac/mkspecs/common/unix.conf \
		/opt/local/libexec/qt4-mac/mkspecs/common/mac.conf \
		/opt/local/libexec/qt4-mac/mkspecs/common/mac-g++.conf \
		/opt/local/libexec/qt4-mac/mkspecs/qconfig.pri \
		/opt/local/libexec/qt4-mac/mkspecs/features/qt_functions.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/qt_config.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/exclusive_builds.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/default_pre.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/default_pre.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/debug.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/default_post.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/default_post.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/objective_c.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/unix/opengl.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/warn_on.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/qt.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/unix/thread.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/moc.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/rez.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/mac/sdk.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/resources.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/uic.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/yacc.prf \
		/opt/local/libexec/qt4-mac/mkspecs/features/lex.prf \
		/opt/local/libexec/qt4-mac/lib/QtOpenGL.framework/QtOpenGL.prl \
		/opt/local/libexec/qt4-mac/lib/QtGui.framework/QtGui.prl \
		/opt/local/libexec/qt4-mac/lib/QtCore.framework/QtCore.prl
	$(QMAKE) -macx -o Makefile project1.pro
/opt/local/libexec/qt4-mac/mkspecs/common/unix.conf:
/opt/local/libexec/qt4-mac/mkspecs/common/mac.conf:
/opt/local/libexec/qt4-mac/mkspecs/common/mac-g++.conf:
/opt/local/libexec/qt4-mac/mkspecs/qconfig.pri:
/opt/local/libexec/qt4-mac/mkspecs/features/qt_functions.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/qt_config.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/exclusive_builds.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/default_pre.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/mac/default_pre.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/debug.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/default_post.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/mac/default_post.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/mac/objective_c.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/unix/opengl.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/warn_on.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/qt.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/unix/thread.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/moc.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/mac/rez.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/mac/sdk.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/resources.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/uic.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/yacc.prf:
/opt/local/libexec/qt4-mac/mkspecs/features/lex.prf:
/opt/local/libexec/qt4-mac/lib/QtOpenGL.framework/QtOpenGL.prl:
/opt/local/libexec/qt4-mac/lib/QtGui.framework/QtGui.prl:
/opt/local/libexec/qt4-mac/lib/QtCore.framework/QtCore.prl:
qmake:  FORCE
	@$(QMAKE) -macx -o Makefile project1.pro

bin/project1.app/Contents/PkgInfo: 
	@$(CHK_DIR_EXISTS) bin/project1.app/Contents || $(MKDIR) bin/project1.app/Contents 
	@$(DEL_FILE) bin/project1.app/Contents/PkgInfo
	@echo "APPL????" >bin/project1.app/Contents/PkgInfo
bin/project1.app/Contents/Info.plist: 
	@$(CHK_DIR_EXISTS) bin/project1.app/Contents || $(MKDIR) bin/project1.app/Contents 
	@$(DEL_FILE) bin/project1.app/Contents/Info.plist
	@sed -e "s,@ICON@,,g" -e "s,@EXECUTABLE@,project1,g" -e "s,@TYPEINFO@,????,g" /opt/local/libexec/qt4-mac/mkspecs/macx-g++/Info.plist.app >bin/project1.app/Contents/Info.plist
dist: 
	@$(CHK_DIR_EXISTS) build/project11.0.0 || $(MKDIR) build/project11.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/project11.0.0/ && $(COPY_FILE) --parents src/re167/BasicMath.h src/re167/Camera.h src/re167/Frustum.h src/re167/GLRenderContext.h src/re167/GLRenderWidget.h src/re167/GLWidget.h src/main/MainWidget.h src/main/MainWindow.h src/re167/Matrix4.h src/re167/Object.h src/re167/RenderContext.h src/re167/RenderWidget.h src/main/RenderWidget0.h src/re167/SceneManager.h src/re167/Singleton.h src/re167/Vector3.h src/re167/Vector4.h src/re167/VertexData.h src/re167/VertexDeclaration.h src/re167/RE167_global.h src/main/ColorFactory.h src/main/GeometryFactory.h src/re167/SWWidget.h src/re167/Vector3.h src/re167/Vector4.h src/re167/Frustum.h src/re167/Camera.h src/re167/PGMReader.h src/re167/ObjReader.h src/re167/SWRenderWidget.h src/re167/SWZBuffer.h src/re167/SWRenderContext.h src/re167/Light.h src/re167/Texture.h src/re167/Material.h src/re167/Materials.h src/re167/Shader.h src/re167/FirstPersonCamera.h src/re167/scenegraph/Node.h src/re167/scenegraph/Group.h src/re167/scenegraph/TransformGroup.h src/re167/scenegraph/Leaf.h src/re167/scenegraph/Shape3D.h src/re167/Plane.h src/re167/scenegraph/LightNode.h src/re167/scenegraph/CameraNode.h src/re167/Basis.h src/re167/spline/Spline.h src/re167/spline/BezierCurve.h src/re167/spline/Helix.h src/re167/spline/Morpher.h src/re167/spline/Square.h src/re167/spline/PiecewiseSpline.h build/project11.0.0/ && $(COPY_FILE) --parents src/re167/BasicMath.cpp src/re167/GLRenderContext.cpp src/re167/GLWidget.cpp src/main/main.cpp src/main/MainWidget.cpp src/main/MainWindow.cpp src/re167/Matrix4.cpp src/re167/RenderContext.cpp src/main/RenderWidget0.cpp src/re167/SceneManager.cpp src/re167/VertexData.cpp src/re167/VertexDeclaration.cpp src/main/ColorFactory.cpp src/main/GeometryFactory.cpp src/re167/SWWidget.cpp src/re167/Vector3.cpp src/re167/Vector4.cpp src/re167/Frustum.cpp src/re167/Camera.cpp src/re167/PGMReader.cpp src/re167/ObjReader.cpp src/re167/SWRenderWidget.cpp src/re167/SWZBuffer.cpp src/re167/SWRenderContext.cpp src/re167/Texture.cpp src/re167/Light.cpp src/re167/Material.cpp src/re167/Shader.cpp src/re167/FirstPersonCamera.cpp src/re167/scenegraph/Node.cpp src/re167/scenegraph/Group.cpp src/re167/scenegraph/TransformGroup.cpp src/re167/scenegraph/Leaf.cpp src/re167/scenegraph/Shape3D.cpp src/re167/Plane.cpp src/re167/scenegraph/LightNode.cpp src/re167/scenegraph/CameraNode.cpp src/re167/spline/BezierCurve.cpp src/re167/spline/Spline.cpp src/re167/spline/Circle.cpp src/re167/spline/Helix.cpp src/re167/spline/Morpher.cpp src/re167/Basis.cpp src/re167/spline/Square.cpp src/re167/spline/PiecewiseSpline.cpp build/project11.0.0/ && (cd `dirname build/project11.0.0` && $(TAR) project11.0.0.tar project11.0.0 && $(COMPRESS) project11.0.0.tar) && $(MOVE) `dirname build/project11.0.0`/project11.0.0.tar.gz . && $(DEL_FILE) -r build/project11.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) -r bin/project1.app
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: build/moc_GLWidget.cpp build/moc_MainWindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/moc_GLWidget.cpp build/moc_MainWindow.cpp
build/moc_GLWidget.cpp: src/re167/RE167_global.h \
		src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/GLWidget.h
	/opt/local/libexec/qt4-mac/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ src/re167/GLWidget.h -o build/moc_GLWidget.cpp

build/moc_MainWindow.cpp: src/main/RenderWidget0.h \
		src/re167/GLRenderWidget.h \
		src/re167/RE167_global.h \
		src/re167/RenderWidget.h \
		src/re167/GLWidget.h \
		src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/scenegraph/CameraNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/Node.h \
		src/main/MainWindow.h
	/opt/local/libexec/qt4-mac/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ src/main/MainWindow.h -o build/moc_MainWindow.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

build/BasicMath.o: src/re167/BasicMath.cpp src/re167/BasicMath.h \
		src/re167/RE167_global.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/BasicMath.o src/re167/BasicMath.cpp

build/GLRenderContext.o: src/re167/GLRenderContext.cpp src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RE167_global.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/Material.h \
		src/re167/Texture.h \
		src/re167/Shader.h \
		src/re167/scenegraph/LightNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/Node.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/GLRenderContext.o src/re167/GLRenderContext.cpp

build/GLWidget.o: src/re167/GLWidget.cpp src/re167/GLWidget.h \
		src/re167/RE167_global.h \
		src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/GLRenderWidget.h \
		src/re167/RenderWidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/GLWidget.o src/re167/GLWidget.cpp

build/main.o: src/main/main.cpp src/main/MainWindow.h \
		src/main/RenderWidget0.h \
		src/re167/GLRenderWidget.h \
		src/re167/RE167_global.h \
		src/re167/RenderWidget.h \
		src/re167/GLWidget.h \
		src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/scenegraph/CameraNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/Node.h \
		src/main/GeometryFactory.h \
		src/re167/Basis.h \
		src/re167/scenegraph/Group.h \
		src/re167/spline/BezierCurve.h \
		src/re167/spline/Spline.h \
		src/re167/spline/PiecewiseSpline.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/main.o src/main/main.cpp

build/MainWidget.o: src/main/MainWidget.cpp src/main/MainWidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/MainWidget.o src/main/MainWidget.cpp

build/MainWindow.o: src/main/MainWindow.cpp src/main/MainWindow.h \
		src/main/RenderWidget0.h \
		src/re167/GLRenderWidget.h \
		src/re167/RE167_global.h \
		src/re167/RenderWidget.h \
		src/re167/GLWidget.h \
		src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/scenegraph/CameraNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/Node.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/MainWindow.o src/main/MainWindow.cpp

build/Matrix4.o: src/re167/Matrix4.cpp src/re167/Matrix4.h \
		src/re167/RE167_global.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Matrix4.o src/re167/Matrix4.cpp

build/RenderContext.o: src/re167/RenderContext.cpp src/re167/RenderContext.h \
		src/re167/RE167_global.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/RenderContext.o src/re167/RenderContext.cpp

build/RenderWidget0.o: src/main/RenderWidget0.cpp src/main/RenderWidget0.h \
		src/re167/GLRenderWidget.h \
		src/re167/RE167_global.h \
		src/re167/RenderWidget.h \
		src/re167/GLWidget.h \
		src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/scenegraph/CameraNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/Node.h \
		src/re167/SWWidget.h \
		src/re167/SWRenderContext.h \
		src/re167/SWZBuffer.h \
		src/main/GeometryFactory.h \
		src/re167/Basis.h \
		src/main/ColorFactory.h \
		src/re167/PGMReader.h \
		src/re167/Shader.h \
		src/re167/Materials.h \
		src/re167/SceneManager.h \
		src/re167/FirstPersonCamera.h \
		src/re167/Material.h \
		src/re167/scenegraph/LightNode.h \
		src/re167/scenegraph/Shape3D.h \
		src/re167/scenegraph/TransformGroup.h \
		src/re167/scenegraph/Group.h \
		src/re167/spline/BezierCurve.h \
		src/re167/spline/Spline.h \
		src/re167/spline/Circle.h \
		src/re167/spline/Helix.h \
		src/re167/spline/Morpher.h \
		src/re167/spline/Square.h \
		src/re167/spline/PiecewiseSpline.h \
		src/re167/Texture.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/RenderWidget0.o src/main/RenderWidget0.cpp

build/SceneManager.o: src/re167/SceneManager.cpp src/re167/SceneManager.h \
		src/re167/RE167_global.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Light.h \
		src/re167/FirstPersonCamera.h \
		src/re167/scenegraph/Node.h \
		src/re167/scenegraph/TransformGroup.h \
		src/re167/scenegraph/Group.h \
		src/re167/scenegraph/LightNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/CameraNode.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/SceneManager.o src/re167/SceneManager.cpp

build/VertexData.o: src/re167/VertexData.cpp src/re167/VertexData.h \
		src/re167/RE167_global.h \
		src/re167/VertexDeclaration.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/VertexData.o src/re167/VertexData.cpp

build/VertexDeclaration.o: src/re167/VertexDeclaration.cpp src/re167/VertexDeclaration.h \
		src/re167/RE167_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/VertexDeclaration.o src/re167/VertexDeclaration.cpp

build/ColorFactory.o: src/main/ColorFactory.cpp src/main/ColorFactory.h \
		src/re167/RE167_global.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/ColorFactory.o src/main/ColorFactory.cpp

build/GeometryFactory.o: src/main/GeometryFactory.cpp src/main/GeometryFactory.h \
		src/re167/RE167_global.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Basis.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/main/ColorFactory.h \
		src/re167/ObjReader.h \
		src/re167/PGMReader.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Object.h \
		src/re167/spline/Spline.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/GeometryFactory.o src/main/GeometryFactory.cpp

build/SWWidget.o: src/re167/SWWidget.cpp src/re167/SWWidget.h \
		src/re167/RE167_global.h \
		src/re167/SWRenderContext.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/SWZBuffer.h \
		src/re167/SWRenderWidget.h \
		src/re167/RenderWidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/SWWidget.o src/re167/SWWidget.cpp

build/Vector3.o: src/re167/Vector3.cpp src/re167/Vector3.h \
		src/re167/RE167_global.h \
		src/re167/BasicMath.h \
		src/re167/Vector4.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Vector3.o src/re167/Vector3.cpp

build/Vector4.o: src/re167/Vector4.cpp src/re167/Vector4.h \
		src/re167/RE167_global.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Vector4.o src/re167/Vector4.cpp

build/Frustum.o: src/re167/Frustum.cpp src/re167/Frustum.h \
		src/re167/RE167_global.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Frustum.o src/re167/Frustum.cpp

build/Camera.o: src/re167/Camera.cpp src/re167/Camera.h \
		src/re167/RE167_global.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Camera.o src/re167/Camera.cpp

build/PGMReader.o: src/re167/PGMReader.cpp src/re167/PGMReader.h \
		src/re167/RE167_global.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/PGMReader.o src/re167/PGMReader.cpp

build/ObjReader.o: src/re167/ObjReader.cpp src/re167/ObjReader.h \
		src/re167/RE167_global.h \
		src/main/GeometryFactory.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Basis.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/ObjReader.o src/re167/ObjReader.cpp

build/SWRenderWidget.o: src/re167/SWRenderWidget.cpp src/re167/SWRenderWidget.h \
		src/re167/RE167_global.h \
		src/re167/RenderWidget.h \
		src/re167/SWWidget.h \
		src/re167/SWRenderContext.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/SWZBuffer.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/SWRenderWidget.o src/re167/SWRenderWidget.cpp

build/SWZBuffer.o: src/re167/SWZBuffer.cpp src/re167/SWZBuffer.h \
		src/re167/RE167_global.h \
		src/main/MainWindow.h \
		src/main/RenderWidget0.h \
		src/re167/GLRenderWidget.h \
		src/re167/RenderWidget.h \
		src/re167/GLWidget.h \
		src/re167/GLRenderContext.h \
		src/re167/glew.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/scenegraph/CameraNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/Node.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/SWZBuffer.o src/re167/SWZBuffer.cpp

build/SWRenderContext.o: src/re167/SWRenderContext.cpp src/re167/SWRenderContext.h \
		src/re167/RE167_global.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/SWZBuffer.h \
		src/re167/SWWidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/SWRenderContext.o src/re167/SWRenderContext.cpp

build/Texture.o: src/re167/Texture.cpp src/re167/Texture.h \
		src/re167/RE167_global.h \
		src/re167/glew.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Texture.o src/re167/Texture.cpp

build/Light.o: src/re167/Light.cpp src/re167/Light.h \
		src/re167/RE167_global.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Light.o src/re167/Light.cpp

build/Material.o: src/re167/Material.cpp src/re167/Material.h \
		src/re167/RE167_global.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Texture.h \
		src/re167/Shader.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Material.o src/re167/Material.cpp

build/Shader.o: src/re167/Shader.cpp src/re167/Shader.h \
		src/re167/RE167_global.h \
		src/re167/glew.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Shader.o src/re167/Shader.cpp

build/FirstPersonCamera.o: src/re167/FirstPersonCamera.cpp src/re167/FirstPersonCamera.h \
		src/re167/Camera.h \
		src/re167/RE167_global.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/FirstPersonCamera.o src/re167/FirstPersonCamera.cpp

build/Node.o: src/re167/scenegraph/Node.cpp src/re167/scenegraph/Node.h \
		src/re167/RE167_global.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Node.o src/re167/scenegraph/Node.cpp

build/Group.o: src/re167/scenegraph/Group.cpp src/re167/scenegraph/Group.h \
		src/re167/RE167_global.h \
		src/re167/scenegraph/Node.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h \
		src/re167/scenegraph/TransformGroup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Group.o src/re167/scenegraph/Group.cpp

build/TransformGroup.o: src/re167/scenegraph/TransformGroup.cpp src/re167/scenegraph/TransformGroup.h \
		src/re167/RE167_global.h \
		src/re167/scenegraph/Group.h \
		src/re167/scenegraph/Node.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/TransformGroup.o src/re167/scenegraph/TransformGroup.cpp

build/Leaf.o: src/re167/scenegraph/Leaf.cpp src/re167/scenegraph/Leaf.h \
		src/re167/RE167_global.h \
		src/re167/scenegraph/Node.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Leaf.o src/re167/scenegraph/Leaf.cpp

build/Shape3D.o: src/re167/scenegraph/Shape3D.cpp src/re167/scenegraph/Shape3D.h \
		src/re167/RE167_global.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/scenegraph/Node.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Shape3D.o src/re167/scenegraph/Shape3D.cpp

build/Plane.o: src/re167/Plane.cpp src/re167/Plane.h \
		src/re167/RE167_global.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Plane.o src/re167/Plane.cpp

build/LightNode.o: src/re167/scenegraph/LightNode.cpp src/re167/scenegraph/LightNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/RE167_global.h \
		src/re167/scenegraph/Node.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/LightNode.o src/re167/scenegraph/LightNode.cpp

build/CameraNode.o: src/re167/scenegraph/CameraNode.cpp src/re167/scenegraph/CameraNode.h \
		src/re167/scenegraph/Leaf.h \
		src/re167/RE167_global.h \
		src/re167/scenegraph/Node.h \
		src/re167/RenderContext.h \
		src/re167/Singleton.h \
		src/re167/Camera.h \
		src/re167/Frustum.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Plane.h \
		src/re167/Object.h \
		src/re167/VertexData.h \
		src/re167/VertexDeclaration.h \
		src/re167/Light.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/CameraNode.o src/re167/scenegraph/CameraNode.cpp

build/BezierCurve.o: src/re167/spline/BezierCurve.cpp src/re167/spline/BezierCurve.h \
		src/re167/RE167_global.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/spline/Spline.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/BezierCurve.o src/re167/spline/BezierCurve.cpp

build/Spline.o: src/re167/spline/Spline.cpp src/re167/spline/Spline.h \
		src/re167/RE167_global.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Basis.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Spline.o src/re167/spline/Spline.cpp

build/Circle.o: src/re167/spline/Circle.cpp src/re167/spline/Circle.h \
		src/re167/RE167_global.h \
		src/re167/spline/Spline.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Circle.o src/re167/spline/Circle.cpp

build/Helix.o: src/re167/spline/Helix.cpp src/re167/spline/Helix.h \
		src/re167/RE167_global.h \
		src/re167/spline/Spline.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/spline/BezierCurve.h \
		src/re167/spline/Circle.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Helix.o src/re167/spline/Helix.cpp

build/Morpher.o: src/re167/spline/Morpher.cpp src/re167/spline/Morpher.h \
		src/re167/RE167_global.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/spline/Spline.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Morpher.o src/re167/spline/Morpher.cpp

build/Basis.o: src/re167/Basis.cpp src/re167/Basis.h \
		src/re167/RE167_global.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Basis.o src/re167/Basis.cpp

build/Square.o: src/re167/spline/Square.cpp src/re167/spline/Square.h \
		src/re167/RE167_global.h \
		src/re167/spline/Spline.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Square.o src/re167/spline/Square.cpp

build/PiecewiseSpline.o: src/re167/spline/PiecewiseSpline.cpp src/re167/spline/PiecewiseSpline.h \
		src/re167/RE167_global.h \
		src/re167/spline/Spline.h \
		src/re167/Matrix4.h \
		src/re167/Vector4.h \
		src/re167/Vector3.h \
		src/re167/BasicMath.h \
		src/re167/spline/Square.h \
		src/re167/spline/Circle.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/PiecewiseSpline.o src/re167/spline/PiecewiseSpline.cpp

build/moc_GLWidget.o: build/moc_GLWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_GLWidget.o build/moc_GLWidget.cpp

build/moc_MainWindow.o: build/moc_MainWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_MainWindow.o build/moc_MainWindow.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

