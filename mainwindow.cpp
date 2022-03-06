#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/shellpool.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    utils::ShellPool<utils::SHELL_BASH>::getInstance().setParent(this);

    a = new RoscoreWidget(this);
    a->setGeometry(0,0,500,500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

