#ifndef SLAMWIDGET_H
#define SLAMWIDGET_H

#include "launchwidget.h"
#include "sensorwidget.h"
#include "slamlaunchtableview.h"

const QString DefaultSlamName = "SLAM";

class SlamWidget : public LaunchWidget
{
    Q_OBJECT
public:
    explicit SlamWidget(QWidget *parent = nullptr, const QString &name = "SLAM");
    virtual void saveCurrentConfig(QSettings *settings, const QString &group, int index);
    virtual void loadConfig(QSettings *settings, const QString &group, int index);
    void setImuWidget(LaunchWidget *wid);
    void setLidarWidget(LaunchWidget *wid);
    void setCameraWidget(LaunchWidget *wid);
    void setSensorWidgetArray(std::vector<std::unordered_set<SensorWidget*>> *wid_array);

private:
    std::vector<std::unordered_set<SensorWidget*>> *sensor_widget_array = nullptr;

    LaunchWidget *imu_widget = nullptr;
    LaunchWidget *lidar_widget = nullptr;
    LaunchWidget *camera_widget = nullptr;

    QAnimationGroup *animation_blink_gourp;

private slots:
    void onSlamChanged(int index);

};

#endif // SLAMWIDGET_H
