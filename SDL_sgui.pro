# -------------------------------------------------
# Project created by QtCreator 2010-07-24T16:54:05
# -------------------------------------------------
QT -= core \
    gui
TARGET = SDL_sgui
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += src/main.cpp
win32:LIBS = -lmingw32 -lSDLmain
LIBS += -lSDL \
	-lSDL_image \
	libSDL_sgui.a
INCLUDEPATH += include
QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_RELEASE = -Os \
    -march=native \
    -mtune=native
