QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    components/lib/qtmaterialcheckable.cpp \
    components/lib/qtmaterialcheckable_internal.cpp \
    components/lib/qtmaterialoverlaywidget.cpp \
    components/lib/qtmaterialripple.cpp \
    components/lib/qtmaterialrippleoverlay.cpp \
    components/lib/qtmaterialstatetransition.cpp \
    components/lib/qtmaterialstyle.cpp \
    components/lib/qtmaterialtheme.cpp \
    components/qtmaterialcircularprogress.cpp \
    components/qtmaterialcircularprogress_internal.cpp \
    components/qtmaterialdialog.cpp \
    components/qtmaterialdialog_internal.cpp \
    components/qtmaterialflatbutton.cpp \
    components/qtmaterialflatbutton_internal.cpp \
    components/qtmaterialraisedbutton.cpp \
    components/qtmaterialsnackbar.cpp \
    components/qtmaterialsnackbar_internal.cpp \
    components/qtmaterialtextfield.cpp \
    components/qtmaterialtextfield_internal.cpp \
    components/qtmaterialtoggle.cpp \
    components/qtmaterialtoggle_internal.cpp \
    filedelegate.cpp \
    launchconfigdialog.cpp \
    launchtableview.cpp \
    main.cpp \
    mainwindow.cpp \
    roscorewidget.cpp \
    sensorwidget.cpp \
    utils/sys.cpp

HEADERS += \
    components/lib/qtmaterialcheckable.h \
    components/lib/qtmaterialcheckable_internal.h \
    components/lib/qtmaterialcheckable_p.h \
    components/lib/qtmaterialoverlaywidget.h \
    components/lib/qtmaterialripple.h \
    components/lib/qtmaterialrippleoverlay.h \
    components/lib/qtmaterialstatetransition.h \
    components/lib/qtmaterialstatetransitionevent.h \
    components/lib/qtmaterialstyle.h \
    components/lib/qtmaterialstyle_p.h \
    components/lib/qtmaterialtheme.h \
    components/lib/qtmaterialtheme_p.h \
    components/qtmaterialcircularprogress.h \
    components/qtmaterialcircularprogress_internal.h \
    components/qtmaterialcircularprogress_p.h \
    components/qtmaterialdialog.h \
    components/qtmaterialdialog_internal.h \
    components/qtmaterialdialog_p.h \
    components/qtmaterialflatbutton.h \
    components/qtmaterialflatbutton_internal.h \
    components/qtmaterialflatbutton_p.h \
    components/qtmaterialraisedbutton.h \
    components/qtmaterialraisedbutton_p.h \
    components/qtmaterialsnackbar.h \
    components/qtmaterialsnackbar_internal.h \
    components/qtmaterialsnackbar_p.h \
    components/qtmaterialtextfield.h \
    components/qtmaterialtextfield_internal.h \
    components/qtmaterialtextfield_p.h \
    components/qtmaterialtoggle.h \
    components/qtmaterialtoggle_internal.h \
    components/qtmaterialtoggle_p.h \
    filedelegate.h \
    launchconfigdialog.h \
    launchtableview.h \
    mainwindow.h \
    roscorewidget.h \
    sensorwidget.h \
    utils/shellpool.h \
    utils/sys.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    SlamInterface_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
