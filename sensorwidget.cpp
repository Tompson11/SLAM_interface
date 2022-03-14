#include "sensorwidget.h"

SensorWidget::SensorWidget(QWidget *parent, const SensorType &type, const QColor& unact_color, const QColor& act_color) : LaunchWidget(parent, unact_color, act_color)
{
    switch (type) {
    case SensorType::CAMERA: {
        label_title->setText("CAMERA");
        label_main_icon->setStyleSheet("QLabel{"
                                 "image:url(:/icons/icons/slam_interface/svg/camera.svg);"
                                 "}");
        break;
    }
    case SensorType::IMU: {
        label_title->setText("IMU");
        label_main_icon->setStyleSheet("QLabel{"
                                 "image:url(:/icons/icons/slam_interface/svg/imu.svg);"
                                 "}");
        break;
    }
    default: {
        label_title->setText("LIDAR");
        label_main_icon->setStyleSheet("QLabel{"
                                 "image:url(:/icons/icons/slam_interface/svg/lidar.svg);"
                                 "}");
        break;
    }
    }


    label_launch_items->setText("Sensor");
    label_launch_items->setFixedWidth(50);

    label_topic->setFixedWidth(50);

    table_in_dialog = new SensorLaunchTableView(dialog_config);

    int total_width = 900;
    table_in_dialog->setColumnWidth(0, 0.2 * total_width);
    table_in_dialog->setColumnWidth(1, 0.4 * total_width);
    table_in_dialog->setColumnWidth(2, 0.4 * total_width);

    dialog_layout->addWidget(table_in_dialog, 0, 0, 3, 1);
    dialog_config->setTableView(table_in_dialog);
    connect(table_in_dialog, SIGNAL(launchTableUpdate()), this, SLOT(updateLaunchCombo()));


    /*
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(toggle_start, 0, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(label_title, 0, 1, 1, 2, Qt::AlignCenter);
    layout->addWidget(progress_open, 1, 0, 2, 1);
    layout->addWidget(label_main_icon, 1, 0, 2, 1);
    layout->addWidget(label_launch_items, 1, 1, 1, 1);
    layout->addWidget(combo_launch_items, 1, 2, 1, 1);
    layout->addWidget(label_topic, 2, 1, 1, 1);
    layout->addWidget(combo_topic, 2, 2, 1, 1);
    layout->addWidget(button_config, 3, 0 ,1, 1);
    layout->addWidget(checkbox_hz, 3, 1 ,1, 1);
    layout->addWidget(label_hz, 3, 2 ,1, 1);

    mainframe = new QFrame(this);
    mainframe->setFrameStyle(QFrame::StyledPanel);
    mainframe->setLayout(layout);
    mainframe->setFrameShadow(QFrame::Sunken);
    QGridLayout *mainlayout = new QGridLayout();
    mainlayout->addWidget(mainframe);
    this->setLayout(mainlayout);


    button_add_in_dialog = new QtMaterialRaisedButton(dialog_config);
    button_add_in_dialog->setText("Add");

    button_delete_in_dialog = new QtMaterialRaisedButton(dialog_config);
    button_delete_in_dialog->setText("Delete");

    QGridLayout *dialog_layout = new QGridLayout(dialog_config);
    dialog_layout->addWidget(table_in_dialog, 0, 0, 3, 1);
    dialog_layout->addWidget(button_add_in_dialog, 0, 1, 1, 1);
    dialog_layout->addWidget(button_delete_in_dialog, 1, 1, 1, 1);
    dialog_config->setLayout(dialog_layout);

    connect(button_config, &QtMaterialRaisedButton::clicked, this, &SensorWidget::onButtonConfigureClicked);
    connect(button_add_in_dialog, &QtMaterialRaisedButton::clicked, this, &SensorWidget::onButtonAddClicked);
    connect(button_delete_in_dialog, &QtMaterialRaisedButton::clicked, this, &SensorWidget::onButtonDeleteClicked);

    connect(toggle_start, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
    connect(checkbox_hz, SIGNAL(toggled(bool)), this, SLOT(onHzChecked(bool)));

    connect(table_in_dialog, SIGNAL(launchTableUpdate()), this, SLOT(updateLaunchCombo()));
//    connect(combo_topic, SIGNAL(focusIn()), this, SLOT(updateTopicCombo()));
    connect(combo_topic, SIGNAL(currentTextChanged(QString)), this, SLOT(onTopicChanged(QString)));

    connect(&timer_topic, &QTimer::timeout, this, &SensorWidget::onHzOutput);
    connect(&timer_roslaunch_detect, &QTimer::timeout, this, &SensorWidget::detectRoslaunchResult);
    */
}

void SensorWidget::saveCurrentConfig(QSettings *settings, const QString &group) {
    if(settings) {
        auto *model = table_in_dialog->model();

        settings->beginGroup(group);
        settings->beginWriteArray("LAUNCH_FILE");
        for (int i = 0; i < model->rowCount(); i++) {
            settings->setArrayIndex(i);
            settings->setValue("SENSOR_NAME", model->data(model->index(i, 0)).toString());
            settings->setValue("WORKSPACE", model->data(model->index(i, 1)).toString());
            settings->setValue("FILENAME", model->data(model->index(i, 2)).toString());
        }
        settings->endArray();
        settings->endGroup();
    }
}

void SensorWidget::loadConfig(QSettings *settings, const QString &group) {
    if(settings) {
        QFont key_font;

        settings->beginGroup(group);
        int size = settings->beginReadArray("LAUNCH_FILE");
        for (int i = 0; i < size; ++i) {
            QList<QStandardItem*> item;
            item << new QStandardItem("") << new QStandardItem("") << new QStandardItem("");

            settings->setArrayIndex(i);
            item.at(0)->setText(settings->value("SENSOR_NAME").toString());
            item.at(1)->setText(settings->value("WORKSPACE").toString());
            item.at(2)->setText(settings->value("FILENAME").toString());

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
