#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <roscorewidget.h>
#include <sensorwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    RoscoreWidget *roscore_widget;
    SensorWidget *camera_widget;
    SensorWidget *lidar_widget;
    SensorWidget *imu_widget;
};
#endif // MAINWINDOW_H
