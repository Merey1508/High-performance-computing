TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        minirt_test.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../minirt/release/ -lminirt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../minirt/debug/ -lminirt
else:unix: LIBS += -L$$OUT_PWD/../minirt/ -lminirt

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../minirt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../minirt/release/libminirt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../minirt/debug/libminirt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../minirt/release/minirt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../minirt/debug/minirt.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../minirt/libminirt.a
