#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <roscorewidget.h>
#include <sensorwidget.h>
#include <slamwidget.h>
#include <QSettings>
#include <QMessageBox>
#include <QScrollArea>
#include <QMenuBar>
#include <QAction>
#include "grouplaunchwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    void saveCurrentConfig();
    void loadConfig(const QString &setting_name);

    Ui::MainWindow *ui;

    QMenuBar *main_menu;
    QMenu *menu_file;
    QAction *action_open_config;
    QAction *action_save_config;
    QAction *action_saveas_config;

    QFileDialog *dialog_open_config;
    QFileDialog *dialog_saveas_config;

    RoscoreWidget *roscore_widget;

    GroupLaunchWidget *sensor_group_widget;
    GroupLaunchWidget *tool_group_widget;

    QScrollArea *scroll_area_sensor_group;
    QScrollArea *scroll_area_tool_group;

    std::vector<std::unordered_set<SensorWidget*>> sensor_widget_array;
    std::unordered_set<SlamWidget*> slam_widget_array;

    SensorWidget *camera_widget;
    SensorWidget *lidar_widget;
    SensorWidget *imu_widget;
    SlamWidget *slam_widget;

    QString default_setting_path;
    QString default_setting_name;
    QString current_setting_path;
    QString current_setting_name;

protected slots:
    void onNewLaunchWidgetAdded(LaunchWidget *wid);
    void onLaunchWidgetRemoved(LaunchWidget *wid);
    void onOpenConfigClicked();
    void onSaveAsConfigClicked();
};
#endif // MAINWINDOW_H
