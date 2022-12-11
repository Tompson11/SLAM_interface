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
    components/qtmaterialavatar.cpp \
    components/qtmaterialbadge.cpp \
    components/qtmaterialcheckbox.cpp \
    components/qtmaterialcircularprogress.cpp \
    components/qtmaterialcircularprogress_internal.cpp \
    components/qtmaterialdialog.cpp \
    components/qtmaterialdialog_internal.cpp \
    components/qtmaterialdrawer.cpp \
    components/qtmaterialdrawer_internal.cpp \
    components/qtmaterialfab.cpp \
    components/qtmaterialflatbutton.cpp \
    components/qtmaterialflatbutton_internal.cpp \
    components/qtmaterialraisedbutton.cpp \
    components/qtmaterialscrollbar.cpp \
    components/qtmaterialscrollbar_internal.cpp \
    components/qtmaterialslider.cpp \
    components/qtmaterialslider_internal.cpp \
    components/qtmaterialsnackbar.cpp \
    components/qtmaterialsnackbar_internal.cpp \
    components/qtmaterialtextfield.cpp \
    components/qtmaterialtextfield_internal.cpp \
    components/qtmaterialtoggle.cpp \
    components/qtmaterialtoggle_internal.cpp \
    utils/reg.cpp \
    utils/sys.cpp \
    main.cpp \
    widget/clickablebadge.cpp \
    widget/cmddelegate.cpp \
    widget/cmdwidget.cpp \
    widget/errorbadgewidget.cpp \
    widget/filedelegate.cpp \
    widget/focusincombobox.cpp \
    widget/groupcmdwidget.cpp \
    widget/grouplaunchwidget.cpp \
    widget/launchconfigdialog.cpp \
    widget/launchfilereader.cpp \
    widget/launchparamdelegate.cpp \
    widget/launchtableview.cpp \
    widget/launchwidget.cpp \
    widget/mainwindow.cpp \
    widget/roscorewidget.cpp \
    widget/sensorgrouplaunchwidget.cpp \
    widget/sensorlaunchtableview.cpp \
    widget/sensorwidget.cpp \
    widget/slamlaunchtableview.cpp \
    widget/slamwidget.cpp \
    widget/titlewidget.cpp

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
    components/qtmaterialavatar.h \
    components/qtmaterialavatar_p.h \
    components/qtmaterialbadge.h \
    components/qtmaterialbadge_p.h \
    components/qtmaterialcheckbox.h \
    components/qtmaterialcheckbox_p.h \
    components/qtmaterialcircularprogress.h \
    components/qtmaterialcircularprogress_internal.h \
    components/qtmaterialcircularprogress_p.h \
    components/qtmaterialdialog.h \
    components/qtmaterialdialog_internal.h \
    components/qtmaterialdialog_p.h \
    components/qtmaterialdrawer.h \
    components/qtmaterialdrawer_internal.h \
    components/qtmaterialdrawer_p.h \
    components/qtmaterialfab.h \
    components/qtmaterialfab_p.h \
    components/qtmaterialflatbutton.h \
    components/qtmaterialflatbutton_internal.h \
    components/qtmaterialflatbutton_p.h \
    components/qtmaterialraisedbutton.h \
    components/qtmaterialraisedbutton_p.h \
    components/qtmaterialscrollbar.h \
    components/qtmaterialscrollbar_internal.h \
    components/qtmaterialscrollbar_p.h \
    components/qtmaterialslider.h \
    components/qtmaterialslider_internal.h \
    components/qtmaterialslider_p.h \
    components/qtmaterialsnackbar.h \
    components/qtmaterialsnackbar_internal.h \
    components/qtmaterialsnackbar_p.h \
    components/qtmaterialtextfield.h \
    components/qtmaterialtextfield_internal.h \
    components/qtmaterialtextfield_p.h \
    components/qtmaterialtoggle.h \
    components/qtmaterialtoggle_internal.h \
    components/qtmaterialtoggle_p.h \
    utils/reg.h \
    utils/shellpool.h \
    utils/sys.h \
    widget/clickablebadge.h \
    widget/cmddelegate.h \
    widget/cmdwidget.h \
    widget/errorbadgewidget.h \
    widget/filedelegate.h \
    widget/focusincombobox.h \
    widget/groupcmdwidget.h \
    widget/grouplaunchwidget.h \
    widget/launchconfigdialog.h \
    widget/launchfilereader.h \
    widget/launchparamdelegate.h \
    widget/launchtableview.h \
    widget/launchwidget.h \
    widget/mainwindow.h \
    widget/roscorewidget.h \
    widget/sensorgrouplaunchwidget.h \
    widget/sensorlaunchtableview.h \
    widget/sensorwidget.h \
    widget/slamlaunchtableview.h \
    widget/slamwidget.h \
    widget/titlewidget.h

FORMS += \
    widget/mainwindow.ui

TRANSLATIONS += \
    SlamInterface_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    components/resources.qrc
