#include "slamwidget.h"

SlamWidget::SlamWidget(QWidget *parent, const QColor& unact_color, const QColor& act_color) : LaunchWidget(parent, unact_color, act_color)
{
    label_title->setText("SLAM");
    label_main_icon->setStyleSheet("QLabel{"
                             "image:url(:/icons/icons/slam_interface/svg/slam.svg);"
                             "}");

    label_launch_items->setText("SLAM Algorithm");
    label_launch_items->setFixedWidth(120);

    label_topic->setFixedWidth(120);

    table_in_dialog = new SlamLaunchTableView(dialog_config);
    dialog_layout->addWidget(table_in_dialog, 0, 0, 3, 1);
    dialog_config->setTableView(table_in_dialog);
    connect(table_in_dialog, SIGNAL(launchTableUpdate()), this, SLOT(updateLaunchCombo()));

/*
    label_toggle_back->setFixedWidth(100);
    label_toggle_back->setFixedHeight(10);
//    label_toggle_back->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
//    label_toggle_back->setStyleSheet(
//                            "background-color:#ff6600;"
//                );


    label_title->setText("SLAM");
    label_title_back->setMinimumHeight(10);
//    label_title_back->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
//    label_title_back->setStyleSheet("QLabel{"
//                             "background-color:#ff6600;"
//                             "}"
//                );


    label_main_icon->setFixedWidth(100);
    label_main_icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label_main_icon->setStyleSheet("QLabel{"
                             "image:url(:/icons/icons/slam_interface/svg/slam.svg);"
                             "}");

    button_config->setFixedWidth(100);
    button_config->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    label_launch_items->setText("SLAM Algorithm");
//    label_launch_items->setFixedWidth(150);
    label_launch_items->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//    label_topic->setFixedWidth(150);
    label_topic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGridLayout *title_layout = new QGridLayout();
    title_layout->addWidget(label_toggle_back, 0, 0, 1, 1);
    title_layout->addWidget(toggle_start, 0, 0, 1, 1, Qt::AlignCenter);
    title_layout->addWidget(label_title_back, 0, 1, 1, 3);
    title_layout->addWidget(label_title, 0, 1, 1, 3, Qt::AlignCenter);
    title_layout->setSpacing(0);

    QFrame *title_frame = new QFrame(this);
    title_frame->setLayout(title_layout);
    title_frame->setStyleSheet(".QFrame{"
                               "border-top-left-radius: 15px;"
                               "border-top-right-radius: 15px;"
                               "background-color:#ff6600;"
                               "}"
                               );


    QGridLayout *layout = new QGridLayout();
    layout->addWidget(label_main_icon, 0, 0, 2, 1, Qt::AlignCenter);
    layout->addWidget(progress_open, 0, 0, 2, 1, Qt::AlignCenter);
    layout->addWidget(label_launch_items, 0, 1, 1, 1, Qt::AlignVCenter);
    layout->addWidget(combo_launch_items, 0, 2, 1, 1);
    layout->addWidget(label_topic, 1, 1, 1, 1, Qt::AlignVCenter);
    layout->addWidget(combo_topic, 1, 2, 1, 1);
    layout->addWidget(button_config, 2, 0 ,1, 1);
    layout->addWidget(checkbox_hz, 2, 1 ,1, 1);
    layout->addWidget(label_hz, 2, 2 ,1, 1);
    layout->setSpacing(20);
    layout->setMargin(10);


    QFrame *backbone_frame = new QFrame(this);
    backbone_frame->setLayout(layout);
    backbone_frame->setStyleSheet(".QFrame{border:2px solid #ff6600;}"
                                  ".QFrame:hover{border:2px solid #006600;}");

//    QVBoxLayout *total_layout = new QVBoxLayout();
//    total_layout->addLayout(title_layout);
//    total_layout->addLayout(layout);
//    total_layout->setMargin(0);

    QVBoxLayout *mainlayout = new QVBoxLayout();
    mainlayout->addWidget(title_frame, 0);
    mainlayout->addWidget(backbone_frame, 0);
    mainlayout->setSpacing(0);
    this->setLayout(mainlayout);

//    mainframe = new QFrame(this);
////    mainframe->setFrameStyle(QFrame::StyledPanel);
//    mainframe->setLayout(total_layout);
////    mainframe->setFrameShadow(QFrame::Sunken);
//    QGridLayout *mainlayout = new QGridLayout(this);
//    mainlayout->addWidget(mainframe, 0, 0, Qt::AlignTop);
//    this->setLayout(mainlayout);

    table_in_dialog = new SlamLaunchTableView(dialog_config);
    dialog_config->setTableView(table_in_dialog);
    button_add_in_dialog = new QtMaterialRaisedButton(dialog_config);
    button_add_in_dialog->setText("Add");

    button_delete_in_dialog = new QtMaterialRaisedButton(dialog_config);
    button_delete_in_dialog->setText("Delete");

    QGridLayout *dialog_layout = new QGridLayout(dialog_config);
    dialog_layout->addWidget(table_in_dialog, 0, 0, 3, 1);
    dialog_layout->addWidget(button_add_in_dialog, 0, 1, 1, 1);
    dialog_layout->addWidget(button_delete_in_dialog, 1, 1, 1, 1);
    dialog_config->setLayout(dialog_layout);

    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    process_launch = pool.getOneProcess();

    connect(button_config, &QtMaterialRaisedButton::clicked, this, &SlamWidget::onButtonConfigureClicked);
    connect(button_add_in_dialog, &QtMaterialRaisedButton::clicked, this, &SlamWidget::onButtonAddClicked);
    connect(button_delete_in_dialog, &QtMaterialRaisedButton::clicked, this, &SlamWidget::onButtonDeleteClicked);

    connect(toggle_start, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
    connect(checkbox_hz, SIGNAL(toggled(bool)), this, SLOT(onHzChecked(bool)));

    connect(table_in_dialog, SIGNAL(launchTableUpdate()), this, SLOT(updateLaunchCombo()));
//    connect(combo_topic, SIGNAL(focusIn()), this, SLOT(updateTopicCombo()));
    connect(combo_topic, SIGNAL(currentTextChanged(QString)), this, SLOT(onTopicChanged(QString)));

    connect(&timer_topic, &QTimer::timeout, this, &SlamWidget::onHzOutput);
    connect(&timer_roslaunch_detect, &QTimer::timeout, this, &SlamWidget::detectRoslaunchResult);
*/
}

void SlamWidget::saveCurrentConfig(QSettings *settings, const QString &group) {
    if(settings) {
        auto *model = table_in_dialog->model();

        settings->beginGroup(group);
        settings->beginWriteArray("LAUNCH_FILE");
        for (int i = 0; i < model->rowCount(); i++) {
            settings->setArrayIndex(i);
            settings->setValue("SENSOR_NAME", model->data(model->index(i, 0)).toString());
            settings->setValue("WORKSPACE", model->data(model->index(i, 1)).toString());
            settings->setValue("FILENAME", model->data(model->index(i, 2)).toString());
            settings->setValue("LIDAR", model->data(model->index(i, 3)).toBool());
            settings->setValue("CAMERA", model->data(model->index(i, 4)).toBool());
            settings->setValue("IMU", model->data(model->index(i, 5)).toBool());
        }
        settings->endArray();
        settings->endGroup();
    }
}

void SlamWidget::loadConfig(QSettings *settings, const QString &group) {
    if(settings) {
        QList<QStandardItem*> item;
        item << new QStandardItem("") << new QStandardItem("") << new QStandardItem("")
             << new QStandardItem("") << new QStandardItem("") << new QStandardItem("");

        settings->beginGroup(group);
        int size = settings->beginReadArray("LAUNCH_FILE");
        for (int i = 0; i < size; ++i) {
            settings->setArrayIndex(i);
            item.at(0)->setData(settings->value("SENSOR_NAME"));
            item.at(1)->setData(settings->value("WORKSPACE"));
            item.at(2)->setData(settings->value("FILENAME"));
            item.at(3)->setData(settings->value("LIDAR"));
            item.at(4)->setData(settings->value("CAMERA"));
            item.at(5)->setData(settings->value("IMU"));
            table_in_dialog->addRow(i, item);
        }
        settings->endArray();
        settings->endGroup();

        updateLaunchCombo();
    }
}
