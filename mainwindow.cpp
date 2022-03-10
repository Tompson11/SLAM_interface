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

    roscore_widget = new RoscoreWidget(this);
    roscore_widget->loadConfig(settings, "ROSCORE");

    lidar_widget = new SensorWidget(this);
    lidar_widget->setRoscoreWidget(roscore_widget);
    lidar_widget->loadConfig(settings, "LIDAR");

    camera_widget = new SensorWidget(this);
    camera_widget->setRoscoreWidget(roscore_widget);
    camera_widget->loadConfig(settings, "CAMERA");

    imu_widget = new SensorWidget(this);
    imu_widget->setRoscoreWidget(roscore_widget);
    imu_widget->loadConfig(settings, "IMU");

    slam_widget = new SlamWidget(this);
    slam_widget->setRoscoreWidget(roscore_widget);
    slam_widget->loadConfig(settings, "SLAM");

    if(settings)
        delete settings;

    QWidget *mainwidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(mainwidget);
    layout->addWidget(roscore_widget, 0, 0, 1, 3);
    layout->addWidget(lidar_widget, 1, 0, 1, 1);
    layout->addWidget(camera_widget, 1, 1, 1, 1);
    layout->addWidget(imu_widget, 1, 2, 1, 1);
    layout->addWidget(slam_widget, 2, 0, 1, 3);
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

//    auto ret = QMessageBox::question(this, "Exit", "Whether to close the SlamInterface?",
//                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

//    if(ret == QMessageBox::Yes)
//        event->accept();
//    else
//        event->ignore();
}

void MainWindow::saveCurrentConfig() {
//    auto ret = QMessageBox::question(this, "Save Config", "Whether to save the current config?");

//    if(ret == QMessageBox::Yes) {
//        QSettings *settings = new QSettings("setting.ini", QSettings::IniFormat);

//        roscore_widget->saveCurrentConfig(settings, "ROSCORE");
//        lidar_widget->saveCurrentConfig(settings, "LIDAR");
//        camera_widget->saveCurrentConfig(settings, "CAMERA");
//        imu_widget->saveCurrentConfig(settings, "IMU");
//        slam_widget->saveCurrentConfig(settings, "SLAM");

//        delete settings;
//    }
}
