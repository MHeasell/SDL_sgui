# -------------------------------------------------
# Project created by QtCreator 2010-07-24T16:54:05
# -------------------------------------------------
QT -= core \
    gui
TARGET = SDL_sgui
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += src/main.cpp \
    src/widget.cpp \
    src/font.cpp \
    src/window.cpp \
    src/label.cpp \
    src/layout.cpp \
    src/hboxlayout.cpp \
    src/vboxlayout.cpp \
    src/button.cpp \
    src/spacer.cpp \
    src/frame.cpp \
    src/lineinput.cpp \
    src/scrollbar.cpp \
    src/textedit.cpp \
    src/listbox.cpp \
    src/picture.cpp \
    src/checkbox.cpp \
    src/radiobutton.cpp \
    src/group.cpp \
    src/renderapi.cpp \
    src/progressbar.cpp \
    src/spinbox.cpp \
    src/receiver.cpp \
    src/unicode.cpp \
    src/tabwidget.cpp \
    src/scrollarea.cpp \
    src/unmanagedlayout.cpp
LIBS += -lSDL \
    -lSDL_image
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
    src/renderapi.h
QMAKE_CXXFLAGS_DEBUG += -p \
    -O1
QMAKE_LFLAGS_DEBUG += -p
QMAKE_CXXFLAGS_RELEASE = -Os \
    -march=native \
    -mtune=native
