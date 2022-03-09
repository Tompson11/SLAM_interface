#include "sensorwidget.h"

SensorWidget::SensorWidget(QWidget *parent) : QWidget(parent)
{
    label_sensor_icon = new QLabel(this);
    label_sensor_icon->setText("sensor icon");

    toggle_start = new QtMaterialToggle(this);

    label_sensor = new QLabel(this);
    label_sensor->setText("Sensor");

    combo_sensor = new QComboBox(this);

    label_topic =  new QLabel(this);
    label_topic->setText("Topic");

    checkbox_hz = new QtMaterialCheckBox(this);
    checkbox_hz->setText("Hz");

    combo_topic = new FocusInComboBox(this);
    combo_topic->setEnabled(false);

    label_hz = new QLabel(this);

    button_config = new QtMaterialRaisedButton(this);
    button_config->setText("Configure");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(label_sensor_icon, 0, 0, 2, 1);
    layout->addWidget(toggle_start, 2, 0, 1, 1);
    layout->addWidget(label_sensor, 0, 1, 1, 1);
    layout->addWidget(combo_sensor, 0, 2, 1, 1);
    layout->addWidget(label_topic, 1, 1, 1, 1);
    layout->addWidget(combo_topic, 1, 2, 1, 1);
    layout->addWidget(checkbox_hz, 2, 1 ,1, 1);
    layout->addWidget(label_hz, 2, 2 ,1, 1);
    layout->addWidget(button_config, 3, 1 ,1, 1);
    this->setLayout(layout);

    dialog_config = new LaunchConfigDialog(this);

    table_in_dialog = new LaunchTableView(dialog_config);
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

    connect(button_add_in_dialog, &QtMaterialRaisedButton::clicked, this, &SensorWidget::onButtonAddClicked);
    connect(button_delete_in_dialog, &QtMaterialRaisedButton::clicked, this, &SensorWidget::onButtonDeleteClicked);
    connect(button_config, &QtMaterialRaisedButton::clicked, this, &SensorWidget::onButtonConfigureClicked);

    connect(toggle_start, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
    connect(checkbox_hz, SIGNAL(toggled(bool)), this, SLOT(onHzChecked(bool)));

    connect(table_in_dialog, SIGNAL(launchTableUpdate()), this, SLOT(updateSensorCombo()));
    connect(combo_topic, SIGNAL(focusIn()), this, SLOT(updateTopicCombo()));
    connect(combo_topic, SIGNAL(currentTextChanged(QString)), this, SLOT(onTopicChanged(QString)));

    connect(&timer_topic, &QTimer::timeout, this, &SensorWidget::onHzOutput);

    table_in_dialog->loadHistoryConfig();

}

void SensorWidget::setRoscoreWidget(RoscoreWidget *ptr) {
    roscore_widget = ptr;
}

void SensorWidget::onButtonConfigureClicked() {
    dialog_config->exec();
}

void SensorWidget::onButtonAddClicked() {
    QModelIndexList selected_index = table_in_dialog->selectionModel()->selectedIndexes();
    QList<QStandardItem*> list;

    if(!selected_index.isEmpty()) {
        table_in_dialog->addRow(selected_index[0].row() + 1, list);
    }
    else {
        table_in_dialog->addRow(table_in_dialog->model()->rowCount(), list);
    }
}

void SensorWidget::onButtonDeleteClicked() {
    QModelIndexList selected_index = table_in_dialog->selectionModel()->selectedIndexes();

    if(!selected_index.isEmpty()) {
        table_in_dialog->deleteRow(selected_index[0].row());
    }
}

void SensorWidget::onToggled(bool tog) {
    if(tog) {
        int row = combo_sensor->currentIndex();
        auto *model = table_in_dialog->model();

        QString workspace_bash = model->data(model->index(row, 1)).toString() + "/devel/setup.bash";
        if(!utils::existDir(workspace_bash)) {
            QMessageBox::critical(this, "invalid workspace", workspace_bash + "is not valid, please modify it first!");
            return;
        }

        QString cmd;
        if(roscore_widget->getSourceROSCmd(cmd)) {
            QString launch_file = model->data(model->index(row, 2)).toString();
            QStringList arguments;
            cmd += "source " + workspace_bash + ";";
            cmd += "roslaunch " + launch_file + ";";
            arguments << "-c" << cmd;
            process_launch->setArguments(arguments);
            process_launch->start();
        }
        else {
            QMessageBox::critical(this, "invalid roscore path!", "roscore path is not valid, please modify it first!");
        }
    }
    else {
        utils::killSystemProcess(nullptr, "roslaunch", QString::number(process_launch->pid()));
        process_launch->terminate();
        process_launch->waitForFinished();
    }
}

void SensorWidget::updateSensorCombo() {
    QStringList &&all_sensors = table_in_dialog->getAllKeys();
    combo_sensor->clear();
    combo_sensor->addItems(all_sensors);
}

void SensorWidget::onTopicChanged(const QString &text) {
    if(text.isEmpty() || cur_monitored_topic == text)
        return;

    stopMonitorTopicHz();
    monitorTopicHz(text);
}

void SensorWidget::onHzChecked(bool tog) {
    if(tog) {
        combo_topic->setEnabled(true);

        if(!combo_topic->currentText().isEmpty()) {
            monitorTopicHz(combo_topic->currentText());
        }
    }
    else {
        combo_topic->setEnabled(false);

        stopMonitorTopicHz();
    }
}

void SensorWidget::monitorTopicHz(const QString &topic) {
    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    process_topic = pool.getOneProcess();

    QString cmd;
    if(roscore_widget->getSourceROSCmd(cmd)) {
        cur_monitored_topic = topic;

        QStringList arguments;
        cmd += "export PYTHONUNBUFFERED=1;";
        cmd += "rostopic hz " + topic + ";";
        arguments << "-c" << cmd;
        process_topic->setArguments(arguments);
        process_topic->start();

        timer_topic.setInterval(1000);
        timer_topic.start();
    }
}

void SensorWidget::stopMonitorTopicHz() {
    if(process_topic) {
        cur_monitored_topic.clear();

        utils::killSystemProcess(nullptr, "rostopic", QString::number(process_topic->pid()));
        process_topic->terminate();
        process_topic->waitForFinished();

        auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
        pool.returnOneProcess(process_topic);
        process_topic = nullptr;

        timer_topic.stop();
    }
}

void SensorWidget::onHzOutput() {
    QString out_str;
    utils::getQProcessStandardOutput(process_topic, out_str, true);

    QStringList extract_ret = utils::extractNumberAfterString(out_str, "average rate: ");
    if(extract_ret.size() > 0) {
        label_hz->setText(extract_ret[0]);
    }
    else {
        label_hz->setText("0.000");
    }
}

void SensorWidget::updateTopicCombo() {
    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    QProcess *p = pool.getOneProcess();

    QString cmd;
    if(roscore_widget->getSourceROSCmd(cmd)) {
        QStringList arguments;
        cmd += "rostopic list;";
        arguments << "-c" << cmd;
        p->setArguments(arguments);
        p->start();
        p->waitForFinished();

        QString out_str;
        utils::getQProcessStandardOutput(p, out_str, true);

        if(!out_str.isEmpty()) {
            QStringList topic_list = out_str.split("\n");
            topic_list.push_front("");
            combo_topic->clear();
            combo_topic->addItems(topic_list);
        }
    }

    pool.returnOneProcess(p);
}
