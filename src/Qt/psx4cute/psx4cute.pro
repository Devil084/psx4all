TEMPLATE = app
TARGET = psx4cute
QT += core \
    gui # multimedia
    
HEADERS += filewidget.h \
    dpadwidget.h \
	QRemoteControlKeys.h \
	aboutdialog.h \
    controlsettings.h \
    videosettings.h \
    audiosettings.h \
    keyconfigdialog.h \
    viewcontroller.h \
    emusettings.h \
    psxadaptation.h \
    QBlitterWidget.h \
    psxSettings.h \
    psx4Qt.h
    
SOURCES += filewidget.cpp \
    dpadwidget.cpp \
	QRemoteControlKeys.cpp \
	aboutdialog.cpp \
    controlsettings.cpp \
    videosettings.cpp \
    audiosettings.cpp \
    keyconfigdialog.cpp \
    viewcontroller.cpp \
    emusettings.cpp \
    cuteDebug.cpp \
    psxadaptation.cpp \
    QBlitterWidget.cpp \
    main.cpp \
    psx4Qt.cpp \
    bitmapblit.cpp
FORMS += filewidget.ui \
    dpadwidget.ui \
	aboutdialog.ui \
    controlsettings.ui \
    videosettings.ui \
    keyconfigdialog.ui \
    audiosettings.ui \
    emusettings.ui

symbian:LIBS += -lpsx4symb.lib \
    -lscdv \
    -lws32 \
    -lcone \
    -lbitgdi \
    -lmediaclientaudiostream \
    -leikcore \
    -lavkon \
    -lremconcoreapi \
    -lremconinterfacebase \
    -lecom \
    -lmmfcontrollerframework #TODO: remove when audio can be removed..
 symbian:TARGET.UID3 \
    = \
    0xE048D429
symbian:TARGET.EPOCHEAPSIZE = 0x200000 \
    0x800000
    
symbian:TARGET.EPOCSTACKKSIZE = 0x10000
#symbian:TARGET.CAPABILITY += SwEvent
#symbian:ICON = gfx/psx4symb.svg
symbian:INCLUDEPATH += ../../symbian/glue/ \
                       ../../ \
                       ../../symbian/relocator/