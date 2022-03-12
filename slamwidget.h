#ifndef SLAMWIDGET_H
#define SLAMWIDGET_H

#include "launchwidget.h"
#include "slamlaunchtableview.h"

class SlamWidget : public LaunchWidget
{
    Q_OBJECT
public:
    explicit SlamWidget(QWidget *parent = nullptr, const QColor& unact_color = QColor(255,255,255), const QColor& act_color = QColor(255,255,255));
    virtual void saveCurrentConfig(QSettings *settings, const QString &group);
    virtual void loadConfig(QSettings *settings, const QString &group);
};

#endif // SLAMWIDGET_H
