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
    void setImuWidget(LaunchWidget *wid);
    void setLidarWidget(LaunchWidget *wid);
    void setCameraWidget(LaunchWidget *wid);

private:
    LaunchWidget *imu_widget = nullptr;
    LaunchWidget *lidar_widget = nullptr;
    LaunchWidget *camera_widget = nullptr;

    QAnimationGroup *animation_blink_gourp;

private slots:
    void onSlamChanged(int index);

};

#endif // SLAMWIDGET_H
