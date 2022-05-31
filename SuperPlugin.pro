
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
QT += core gui widgets opengl openglwidgets
#CONFIG	+= x86_64
VAA3DPATH = /Users/jazz/Desktop/test/v3d_external
RUNV3DPATH = /Users/jazz/Desktop/vaa3dapp
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/basic_c_fun
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/v3d
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/common_lib/include
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/plugin_loader
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/3drenderer
INCLUDEPATH     += $$V3DMAINPATH

macx{
    DEFINES += MACOS_SYSTEM
}

win32{
    DEFINES += WINDOWS_SYSTEM
}


HEADERS	+= SuperPlugin_plugin.h \
    Computation.h \
    Postprocess.h \
    PreLibrary.h \
    Preprocess.h \
    data_io.h \
    superplugin_ui.h \
    utiltools.h
SOURCES	+= SuperPlugin_plugin.cpp \
    Computation.cpp \
    Postprocess.cpp \
    PreLibrary.cpp \
    Preprocess.cpp \
    data_io.cpp \
    superplugin_ui.cpp \
    utiltools.cpp
SOURCES	+= $$VAA3DPATH/v3d_main/basic_c_fun/v3d_message.cpp
SOURCES	+= $$VAA3DPATH/v3d_main/basic_c_fun/basic_surf_objs.cpp

TARGET	= $$qtLibraryTarget(SuperPlugin)
DESTDIR	= $$RUNV3DPATH/plugins/SuperPlugin/
