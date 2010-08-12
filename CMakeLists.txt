cmake_minimum_required (VERSION 2.6)
project (SDL_sgui)

SET(SOURCES
	src/SDL_sgui.h
	src/types.h
	src/arrowdata.h
	src/checkdata.h
	src/fontdata.h

	src/button.cpp		src/button.h
	src/checkbox.cpp	src/checkbox.h
	src/floatting.cpp	src/floatting.h
	src/font.cpp		src/font.h
	src/frame.cpp		src/frame.h
	src/group.cpp		src/group.h
	src/hboxlayout.cpp	src/hboxlayout.h
	src/label.cpp 		src/label.h
	src/layout.cpp		src/layout.h
	src/lineinput.cpp	src/lineinput.h
	src/listbox.cpp		src/listbox.h
	src/menubar.cpp		src/menubar.h
	src/menu.cpp		src/menu.h
	src/menuentry.cpp	src/menuentry.h     
	src/picture.cpp		src/picture.h
	src/progressbar.cpp	src/progressbar.h
	src/radiobutton.cpp	src/radiobutton.h
	src/receiver.cpp	src/receiver.h
	src/renderapi.cpp	src/renderapi.h
	src/scrollarea.cpp	src/scrollarea.h
	src/scrollbar.cpp	src/scrollbar.h
	src/spacer.cpp		src/spacer.h
	src/spinbox.cpp		src/spinbox.h
	src/tabwidget.cpp	src/tabwidget.h
	src/textedit.cpp	src/textedit.h
	src/unicode.cpp		src/unicode.h
	src/unmanagedlayout.cpp	src/unmanagedlayout.h
	src/utils.cpp		src/utils.h
	src/vboxlayout.cpp	src/vboxlayout.h
	src/widget.cpp		src/widget.h
	src/window.cpp		src/window.h)

SET(CMAKE_CXX_FLAGS_RELEASE -Os)

add_library(SDL_sgui ${SOURCES})
