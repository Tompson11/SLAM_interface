#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include "launchwidget.h"
#include "sensorlaunchtableview.h"

enum SensorType {
    LIDAR = 0,
    CAMERA,
    IMU,
    OTHERS,
    TOOLS
};

const QColor DefaultSensorColorActive[5] = {
    QColor(46, 134, 193),
    QColor(40, 180, 99),
    QColor(243, 156, 18),
    QColor(133, 11, 234),
    QColor(255, 113, 0)
};

const QColor DefaultSensorColorInActive[5] = {
    QColor(174, 214, 241),
    QColor(171, 235, 198),
    QColor(248, 196, 113),
    QColor(196, 149, 235),
    QColor(255, 175, 111)
};

const QString DefaultSensorName[5] = {
    QString("LIDAR"),
    QString("CAMERA"),
    QString("IMU"),
    QString("OTHERS"),
    QString("TOOLS"),
};

class SensorWidget : public LaunchWidget
{
    Q_OBJECT
public:
    explicit SensorWidget(QWidget *parent = nullptr, const SensorType &type = SensorType::LIDAR, const QString &sensor_name = "");
    virtual void saveCurrentConfig(QSettings *settings, const QString &group, int index, int index_in_group);
    virtual void loadConfig(QSettings *settings, const QString &group, int index, int &index_in_group);
    SensorType getSensorType();

protected:
    SensorType type_;

};

#endif // SENSORWIDGET_H
