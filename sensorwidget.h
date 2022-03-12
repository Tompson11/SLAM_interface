#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include "launchwidget.h"
#include "sensorlaunchtableview.h"

class SensorWidget : public LaunchWidget
{
    Q_OBJECT
public:
    enum SensorType {
        LIDAR = 0,
        CAMERA,
        IMU
    };

    explicit SensorWidget(QWidget *parent = nullptr, const SensorType &type = SensorType::LIDAR, const QColor& unact_color = QColor(255,255,255), const QColor& act_color = QColor(255,255,255));
    virtual void saveCurrentConfig(QSettings *settings, const QString &group);
    virtual void loadConfig(QSettings *settings, const QString &group);


};

#endif // SENSORWIDGET_H
