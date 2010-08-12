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
HEADERS += src/widget.h \
    src/fontdata.h \
    src/arrowdata.h \
    src/checkdata.h \
    src/font.h \
    src/window.h \
    src/label.h \
    src/layout.h \
    src/types.h \
    src/hboxlayout.h \
    src/vboxlayout.h \
    src/button.h \
    src/spacer.h \
    src/frame.h \
    src/lineinput.h \
    src/scrollbar.h \
    src/textedit.h \
    src/listbox.h \
    src/picture.h \
    src/checkbox.h \
	src/radiobutton.h \
    src/group.h \
    src/progressbar.h \
    src/spinbox.h \
    src/receiver.h \
    src/unicode.h \
    src/SDL_sgui.h \
    src/tabwidget.h \
    src/scrollarea.h \
    src/unmanagedlayout.h \
    src/renderapi.h \
    src/menubar.h \
    src/menu.h \
    src/floatting.h \
    src/menuentry.h \
    src/utils.h
QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_RELEASE = -Os \
    -march=native \
    -mtune=native
