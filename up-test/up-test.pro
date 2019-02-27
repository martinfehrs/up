TEMPLATE = app

TARGET   = checkup

CONFIG  += console c++1z
CONFIG  -= qt

SOURCES += main.cpp \
           stack_template_test.cpp

HEADERS += stack_template_test.hpp \
           type_traits_tests.hpp \
           type_set_tests.hpp

DEFINES += "WHITEBOX_TESTING"

LIBS    += -lcppunit


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../makeup/release/ -lmakeup
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../makeup/debug/ -lmakeup
else:unix: LIBS += -L$$OUT_PWD/../makeup/ -lmakeup

INCLUDEPATH += $$PWD/../makeup/inc
DEPENDPATH += $$PWD/../makeup/inc

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../makeup/release/libmake.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../up-lib/debug/libmakeup.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../makeup/release/makeup.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../makeup/debug/makeup.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../makeup/libmakeup.a
