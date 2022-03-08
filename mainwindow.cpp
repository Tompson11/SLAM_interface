#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/shellpool.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    utils::ShellPool<utils::SHELL_BASH>::getInstance().setParent(this);

    roscore_widget = new RoscoreWidget(this);

    lidar_widget = new SensorWidget(this);
    lidar_widget->setRoscoreWidget(roscore_widget);

    QWidget *mainwidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(mainwidget);
    layout->addWidget(roscore_widget, 0, 0, 1, 3);
    layout->addWidget(lidar_widget, 1, 0, 1, 1);
    mainwidget->setLayout(layout);
    setCentralWidget(mainwidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

