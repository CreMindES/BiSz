#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T04:23:10
#
#-------------------------------------------------

QT       += core gui sql
contains(QT_VERSION, ^5\\..*\\..*): QT += widgets
QT += qml quick


TARGET = BiSz
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myqsfpmodel.cpp \
    itemnumbersearch.cpp \
    qlineeditwithclearbutton.cpp \
    productdetailswidget.cpp \
    databasemanager.cpp \
    productpreview.cpp \
    clickableimage.cpp \
    optionsdialog.cpp

HEADERS  += mainwindow.h \
    myqsfpmodel.h \
    create_connection.h \
    itemnumbersearch.h \
    qlineeditwithclearbutton.h \
    productdetailswidget.h \
    databasemanager.h \
    productpreview.h \
    clickableimage.h \
    optionsdialog.h

FORMS    +=

RESOURCES += \
    resources.qrc

win32:RC_FILE = BiSz.rc

OTHER_FILES += \
    menu.qml \
    MyButton.qml
