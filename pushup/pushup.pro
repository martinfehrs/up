TEMPLATE = app

TARGET = up-studio

QMAKE_CXXFLAGS += -fconcepts -std=gnu++17

QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
           mainwindow.cpp \
           sessionwindow.cpp \
           dataviewdock.cpp \
           inputbar.cpp \
           highlighter.cpp

HEADERS += mainwindow.hpp \
           sessionwindow.hpp \
           idpool.hpp \
           dataviewdock.hpp \
           inputbar.hpp \
           highlighter.hpp \
           managelayoutsdialog.hpp \
           savelayoutsdialog.hpp \
           publishingstorage.hpp \
    uibackend.hpp

RESOURCES += icons.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../makeup/release/ -lmakeup
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../makeup/debug/ -lmakeup
else:unix: LIBS += -L$$OUT_PWD/../makeup/ -lmakeup

INCLUDEPATH += $$PWD/../makeup/inc

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../makeup/release/libmakeup.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../makeup/debug/libmakeup.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../makeup/release/makeup.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../makeup/debug/makeup.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../makeup/libmakeup.a
