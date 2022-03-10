#ifndef SLAMWIDGET_H
#define SLAMWIDGET_H

#include "launchwidget.h"
#include "slamlaunchtableview.h"

class SlamWidget : public LaunchWidget
{
    Q_OBJECT
public:
    explicit SlamWidget(QWidget *parent = nullptr);
    virtual void saveCurrentConfig(QSettings *settings, const QString &group);
    virtual void loadConfig(QSettings *settings, const QString &group);
};

#endif // SLAMWIDGET_H
