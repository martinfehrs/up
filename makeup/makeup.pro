TEMPLATE = lib

TARGET = makeup

QT -= core gui

CONFIG += staticlib c++1z

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS = $$files(*.hpp, true) \
 inc/up/environment.hpp \
 inc/up/io.hpp

SOURCES += \
 src/coreextwords.cpp \
 src/error.cpp \
 src/interpreter.cpp \
    src/numberrunner.cpp

INCLUDEPATH += $$PWD/../makeup/inc/up

unix {
    target.path = /usr/lib
    INSTALLS += target
}

