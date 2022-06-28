#include "slamwidget.h"
#include <QParallelAnimationGroup>

SlamWidget::SlamWidget(QWidget *parent) : LaunchWidget(parent, QColor(241, 148, 138), QColor(203, 67, 53))
{
    label_title->setText("SLAM");
    label_main_icon->setStyleSheet("QLabel{"
                             "image:url(:/icons/icons/slam_interface/svg/slam.svg);"
                             "}");

    label_launch_items->setText("Algorithm");
    label_launch_items->setFixedWidth(80);

    label_topic->setFixedWidth(120);

    table_in_dialog = new SlamLaunchTableView(dialog_config);
    int total_width = 900;
    table_in_dialog->setColumnWidth(0, 0.15 * total_width);
    table_in_dialog->setColumnWidth(1, 0.3 * total_width);
    table_in_dialog->setColumnWidth(2, 0.3 * total_width);

    dialog_layout->addWidget(table_in_dialog, 0, 0, 3, 1);
    dialog_config->setTableView(table_in_dialog);
    connect(table_in_dialog, SIGNAL(launchTableUpdate()), this, SLOT(updateLaunchCombo()));
    connect(combo_launch_items, SIGNAL(currentIndexChanged(int)), this, SLOT(onSlamChanged(int)));

    animation_blink_gourp = new QParallelAnimationGroup(this);

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

void SlamWidget::saveCurrentConfig(QSettings *settings, const QString &group, int index) {
    if(settings) {
        auto *model = table_in_dialog->model();

        settings->beginGroup(group);
        settings->beginWriteArray("MODULE_CONFIG");
        settings->setArrayIndex(index);
        settings->setValue("MODULE_NAME", this->label_title->text());
        settings->setValue("LAUNCH_ITEM", this->combo_launch_items->currentText());
        settings->setValue("TOPIC", this->combo_topic->currentText());

        QString launch_config_key;
        for(int i = 0; i < 10; i++) {
            launch_config_key.push_back('A' + (rand() % 26));
        }
        settings->setValue("KEY", launch_config_key);
        settings->endArray();
        settings->endGroup();

        settings->beginGroup(launch_config_key);
        settings->beginWriteArray("LAUNCH_CONFIG");
        for (int i = 0; i < model->rowCount(); i++) {
            settings->setArrayIndex(i);
            settings->setValue("ITEM_NAME", model->data(model->index(i, 0)).toString());
            settings->setValue("WORKSPACE", model->data(model->index(i, 1)).toString());
            settings->setValue("FILENAME", model->data(model->index(i, 2)).toString());
            settings->setValue("LIDAR", model->data(model->index(i, 3), Qt::UserRole).toBool());
            settings->setValue("CAMERA", model->data(model->index(i, 4), Qt::UserRole).toBool());
            settings->setValue("IMU", model->data(model->index(i, 5), Qt::UserRole).toBool());
            settings->setValue("OTHERS", model->data(model->index(i, 6), Qt::UserRole).toBool());
        }
        settings->endArray();
        settings->endGroup();
    }
}

void SlamWidget::loadConfig(QSettings *settings, const QString &group, int index) {
    if(settings) {
        QString launch_config_key = "";

        settings->beginGroup(group);
        int size = settings->beginReadArray("MODULE_CONFIG");
        if(index < size) {
            settings->setArrayIndex(index);
            this->label_title->setText(settings->value("MODULE_NAME").toString());
            this->combo_launch_items->setCurrentText(settings->value("LAUNCH_ITEM").toString());
            this->combo_topic->setCurrentText(settings->value("TOPIC").toString());

            launch_config_key = settings->value("KEY").toString();
        }

        settings->endArray();
        settings->endGroup();

        QFont key_font;
        settings->beginGroup(launch_config_key);
        size = settings->beginReadArray("LAUNCH_CONFIG");
        for (int i = 0; i < size; ++i) {
            QList<QStandardItem*> item;
            item << new QStandardItem("") << new QStandardItem("") << new QStandardItem("")
                 << new QStandardItem("") << new QStandardItem("") << new QStandardItem("")
                 << new QStandardItem("");

            settings->setArrayIndex(i);
            item.at(0)->setText(settings->value("ITEM_NAME").toString());
            item.at(1)->setText(settings->value("WORKSPACE").toString());
            item.at(2)->setText(settings->value("FILENAME").toString());
            item.at(3)->setData(settings->value("LIDAR"), Qt::UserRole);
            item.at(4)->setData(settings->value("CAMERA"), Qt::UserRole);
            item.at(5)->setData(settings->value("IMU"), Qt::UserRole);
            item.at(6)->setData(settings->value("OTHERS"), Qt::UserRole);

            if(i == 0) {
                key_font = item.at(0)->font();
                key_font.setBold(true);
            }

            item.at(0)->setFont(key_font);
            item.at(0)->setToolTip(item.at(0)->text());
            item.at(1)->setToolTip(item.at(1)->text());
            item.at(2)->setToolTip(item.at(2)->text());
            table_in_dialog->addRow(i, item);
        }
        settings->endArray();
        settings->endGroup();

        updateLaunchCombo();
    }
}

void SlamWidget::setImuWidget(LaunchWidget *wid) {
    this->imu_widget = wid;
}

void SlamWidget::setLidarWidget(LaunchWidget *wid) {
    this->lidar_widget = wid;
}

void SlamWidget::setCameraWidget(LaunchWidget *wid) {
    this->camera_widget = wid;
}

void SlamWidget::setSensorWidgetArray(std::vector<std::unordered_set<SensorWidget*>> *wid_array) {
    this->sensor_widget_array = wid_array;
}

void SlamWidget::onSlamChanged(int index) {
    if(sensor_widget_array == nullptr)
        return;

    auto *model = table_in_dialog->model();
    for(int i = 0; i < SensorType::OTHERS; i++) {
        bool need_sensor = model->data(model->index(index, i + 3), Qt::UserRole).toBool();

        if(need_sensor) {
            for(SensorWidget* ws: (*sensor_widget_array)[i]) {
                if(ws) {
                    ws->playWinkAnimation();
                }
            }
        }
    }

}
