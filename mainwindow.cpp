#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/shellpool.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    utils::ShellPool<utils::SHELL_BASH>::getInstance().setParent(this);

    QSettings *settings = nullptr;
    if(utils::existDir(QString("setting.ini"))) {
        settings = new QSettings("setting.ini", QSettings::IniFormat);
    }

    roscore_widget = new RoscoreWidget(this, QColor(214, 219, 223), QColor(131, 145, 146));
    roscore_widget->loadConfig(settings, "ROSCORE");

    lidar_widget = new SensorWidget(this, SensorWidget::LIDAR, QColor(174, 214, 241), QColor(46, 134, 193));
    lidar_widget->setRoscoreWidget(roscore_widget);
    lidar_widget->loadConfig(settings, "LIDAR");

    camera_widget = new SensorWidget(this, SensorWidget::CAMERA, QColor(171, 235, 198), QColor(40, 180, 99));
    camera_widget->setRoscoreWidget(roscore_widget);
    camera_widget->loadConfig(settings, "CAMERA");

    imu_widget = new SensorWidget(this, SensorWidget::IMU, QColor(248, 196, 113), QColor(243, 156, 18));
    imu_widget->setRoscoreWidget(roscore_widget);
    imu_widget->loadConfig(settings, "IMU");

    slam_widget = new SlamWidget(this, QColor(241, 148, 138), QColor(203, 67, 53));
    slam_widget->setRoscoreWidget(roscore_widget);
    slam_widget->loadConfig(settings, "SLAM");
    slam_widget->setLidarWidget(lidar_widget);
    slam_widget->setCameraWidget(camera_widget);
    slam_widget->setImuWidget(imu_widget);

    if(settings)
        delete settings;

    QHBoxLayout *sensor_layout = new QHBoxLayout();
    sensor_layout->addWidget(lidar_widget);
    sensor_layout->addWidget(camera_widget);
    sensor_layout->addWidget(imu_widget);

    QWidget *mainwidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mainwidget);
    layout->addWidget(roscore_widget, Qt::AlignTop);
    layout->addLayout(sensor_layout, Qt::AlignTop);
    layout->addWidget(slam_widget,Qt::AlignTop);
    mainwidget->setLayout(layout);
    setCentralWidget(mainwidget);
}

MainWindow::~MainWindow()
{
    saveCurrentConfig();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();

    auto ret = QMessageBox::question(this, "Exit", "Whether to close the SlamInterface?",
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if(ret == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

void MainWindow::saveCurrentConfig() {
    auto ret = QMessageBox::question(this, "Save Config", "Whether to save the current config?");

    if(ret == QMessageBox::Yes) {
        QSettings *settings = new QSettings("setting.ini", QSettings::IniFormat);

        roscore_widget->saveCurrentConfig(settings, "ROSCORE");
        lidar_widget->saveCurrentConfig(settings, "LIDAR");
        camera_widget->saveCurrentConfig(settings, "CAMERA");
        imu_widget->saveCurrentConfig(settings, "IMU");
        slam_widget->saveCurrentConfig(settings, "SLAM");

        delete settings;
    }
}
