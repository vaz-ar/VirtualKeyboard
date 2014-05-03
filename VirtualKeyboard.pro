#-------------------------------------------------
#
#   VirtualKeyboard for Qt 5
#
#   Copyright (c) 2014 Arnaud Vazard
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"), to deal
#   in the Software without restriction, including without limitation the rights
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in all
#   copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#   SOFTWARE.
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = VirtualKeyboard
TEMPLATE = app

CONFIG += c++11

SOURCES +=  src/TEST_VirtualKeyboard.cpp \
            src/main_VirtualKeyboard.cpp \
            src/VirtualKeyboard.cpp

HEADERS  += src/TEST_VirtualKeyboard.h \
            src/VirtualKeyboard.h

FORMS    += ui/TEST_VirtualKeyboard.ui \
            ui/VirtualKeyboard.ui

RESOURCES += resources/resources.qrc

OTHER_FILES += README.md

OBJECTS_DIR =   obj
MOC_DIR =       obj
RCC_DIR =       obj
UI_DIR =        obj

