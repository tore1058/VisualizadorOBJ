QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DisplayFileQt
TEMPLATE = app

SOURCES += \
    casa.cpp \
    main.cpp \
    mainwindow.cpp \
    canvaswidget.cpp \
    matriz.cpp \
    ponto.cpp \
    linha.cpp \
    poligono.cpp \
    objetoobj.cpp \
    camera.cpp

HEADERS += \
    casa.h \
    mainwindow.h \
    canvaswidget.h \
    matriz.h \
    objeto.h \
    ponto.h \
    linha.h \
    poligono.h \
    objetoobj.h \
    transformacao2d.h \
    camera.h

FORMS += mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
