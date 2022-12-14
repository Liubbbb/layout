QT  += core gui widgets network

TARGET = Qt5HttpDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 utf8_source

SOURCES += \
        main.cpp \
    pgmessage.cpp \
    LayoutFile/customitem.cpp \
    LayoutFile/customscene.cpp \
    LayoutFile/dragingrectitem.cpp \
    LayoutFile/layout.cpp \
    LayoutFile/MainEdit.cpp \
    LayoutFile/layoutinf.cpp \
    LayoutFile/newlayoutwidget.cpp \
    LayoutFile/multiview.cpp \
    LayoutFile/publicdefine.cpp

HEADERS += \
    pgmessage.h \
    LayoutFile/customitem.h \
    LayoutFile/customscene.h \
    LayoutFile/dragingrectitem.h \
    LayoutFile/layout.h \
    LayoutFile/MainEdit.h \
    LayoutFile/layoutinf.h \
    LayoutFile/newlayoutwidget.h \
    LayoutFile/multiview.h \
    LayoutFile/publicdefine.h

FORMS += \
    LayoutFile/MainEdit.ui \
    LayoutFile/layoutinf.ui \
    LayoutFile/newlayoutwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
