#include "launchwidget.h"

LaunchWidget::LaunchWidget(QWidget *parent, const QColor& unact_color, const QColor& act_color) :
    TitleWidget(parent, unact_color, act_color)
{
    constructWidget();
    constructLayout();
    connectSignal();

}

void LaunchWidget::constructWidget() {
    const int first_column_width = 100;
    label_toggle_back->setFixedWidth(first_column_width);

    progress_open = new QtMaterialCircularProgress(this);
    progress_open->setVisible(false);

    label_main_icon = new QLabel(this);
    label_main_icon->setMinimumSize(QSize(80,80));
    label_main_icon->setFixedWidth(first_column_width);

    badge = new ErrorBadgeWidget(this);
    badge->setBadgeParent(label_main_icon);
    badge->setBadgeShift(25,25);

    label_launch_items = new QLabel(this);

    combo_launch_items = new QComboBox(this);

    label_topic =  new QLabel(this);
    label_topic->setText("Topic");

    checkbox_hz = new QtMaterialCheckBox(this);
    checkbox_hz->setText("Hz");
    checkbox_hz->setEnabled(false);

    combo_topic = new FocusInComboBox(this);
    combo_topic->setEnabled(false);

    label_hz = new QLabel(this);

    button_config = new QtMaterialRaisedButton(this);
    button_config->setText("Configure");
    button_config->setFixedWidth(first_column_width);

    dialog_config = new LaunchConfigDialog(this);
    dialog_config->setWindowTitle("Sensor Configure");
    dialog_config->resize(1080,720);

    button_add_in_dialog = new QtMaterialRaisedButton(dialog_config);
    button_add_in_dialog->setText("Add");

    button_delete_in_dialog = new QtMaterialRaisedButton(dialog_config);
    button_delete_in_dialog->setText("Delete");

    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    process_launch = pool.getOneProcess();
}

void LaunchWidget::constructLayout() {
    body_layout->addWidget(label_main_icon, 0, 0, 2, 1, Qt::AlignCenter);
    body_layout->addWidget(progress_open, 0, 0, 2, 1, Qt::AlignCenter);
    body_layout->addWidget(label_launch_items, 0, 1, 1, 1, Qt::AlignVCenter);
    body_layout->addWidget(combo_launch_items, 0, 2, 1, 1);
    body_layout->addWidget(label_topic, 1, 1, 1, 1, Qt::AlignVCenter);
    body_layout->addWidget(combo_topic, 1, 2, 1, 1);
    body_layout->addWidget(button_config, 2, 0 ,1, 1);
    body_layout->addWidget(checkbox_hz, 2, 1 ,1, 1);
    body_layout->addWidget(label_hz, 2, 2 ,1, 1);
    body_layout->setSpacing(20);
    body_layout->setMargin(10);

    dialog_layout = new QGridLayout(dialog_config);
    dialog_layout->addWidget(button_add_in_dialog, 0, 1, 1, 1);
    dialog_layout->addWidget(button_delete_in_dialog, 1, 1, 1, 1);
    dialog_config->setLayout(dialog_layout);
}



void LaunchWidget::connectSignal() {
    connect(button_config, &QtMaterialRaisedButton::clicked, this, &LaunchWidget::onButtonConfigureClicked);
    connect(button_add_in_dialog, &QtMaterialRaisedButton::clicked, this, &LaunchWidget::onButtonAddClicked);
    connect(button_delete_in_dialog, &QtMaterialRaisedButton::clicked, this, &LaunchWidget::onButtonDeleteClicked);

    connect(toggle_start, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
    connect(checkbox_hz, SIGNAL(toggled(bool)), this, SLOT(onHzChecked(bool)));


//    connect(combo_topic, SIGNAL(focusIn()), this, SLOT(updateTopicCombo()));
    connect(combo_topic, SIGNAL(currentTextChanged(QString)), this, SLOT(onTopicChanged(QString)));

    connect(&timer_topic, &QTimer::timeout, this, &LaunchWidget::onHzOutput);
    connect(&timer_roslaunch_detect, &QTimer::timeout, this, &LaunchWidget::detectRoslaunchResult);

    connect(process_launch, &QProcess::readyReadStandardError,this,&LaunchWidget::handleRoslaunchError);
}

QAbstractItemModel* LaunchWidget::getTableModel() {
    return table_in_dialog->model();
}

void LaunchWidget::setRoscoreWidget(RoscoreWidget *ptr) {
    roscore_widget = ptr;
}

bool LaunchWidget::isRoscoreOpened() {
    return roscore_widget != nullptr && roscore_widget->isRoscoreOpened();
}

void LaunchWidget::onButtonConfigureClicked() {
    dialog_config->exec();
}

void LaunchWidget::onButtonAddClicked() {
    QModelIndexList selected_index = table_in_dialog->selectionModel()->selectedIndexes();
    QList<QStandardItem*> list;

    if(!selected_index.isEmpty()) {
        table_in_dialog->addRow(selected_index[0].row() + 1, list);
    }
    else {
        table_in_dialog->addRow(table_in_dialog->model()->rowCount(), list);
    }
}

void LaunchWidget::onButtonDeleteClicked() {
    QModelIndexList selected_index = table_in_dialog->selectionModel()->selectedIndexes();

    if(!selected_index.isEmpty()) {
        table_in_dialog->deleteRow(selected_index[0].row());
    }
}

void LaunchWidget::onToggled(bool tog) {
    if(tog) {
        toggle_start->setEnabled(false);
        progress_open->setVisible(true);
        combo_launch_items->setEnabled(false);
        animation_activate->start();

        if(!isRoscoreOpened()) {
            QMessageBox::critical(this, "Roscore Unopened", "Please open the roscore first!");
            onRoslaunchFail(true, "");

            if(roscore_widget != nullptr)
                 roscore_widget->playWinkAnimation();

            return;
        }

        int row = combo_launch_items->currentIndex();
        auto *model = table_in_dialog->model();

        QString workspace = model->data(model->index(row, 1)).toString();
        if(workspace.isEmpty()) {
            QMessageBox::critical(this, "No launch item", "Please select a launch item first!");
            onRoslaunchFail(true, "");
            return;
        }

        QString workspace_bash = workspace + "/devel/setup.bash";
        if(!utils::existDir(workspace_bash)) {
            QMessageBox::critical(this, "invalid workspace", workspace + " is not valid, please modify it first!");
            onRoslaunchFail(true, "");
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

            timer_roslaunch_detect.setInterval(2000);
            timer_roslaunch_detect.start();
        }
        else {
            QMessageBox::critical(this, "invalid roscore path!", "roscore path is not valid, please modify it first!");
            onRoslaunchFail(true, "");
        }
    }
    else {
        toggle_start->setEnabled(false);
        progress_open->setVisible(true);

        utils::killSystemProcess(nullptr, "roslaunch", QString::number(process_launch->pid()));
        process_launch->terminate();
        process_launch->waitForFinished();

        onRoslaunchFail(false, "");
    }
}

void LaunchWidget::onRoslaunchSuccess() {
    progress_open->setVisible(false);
    toggle_start->setEnabled(true);
    checkbox_hz->setEnabled(true);
    combo_launch_items->setEnabled(false);
    combo_topic->setEnabled(true);

    int row = combo_launch_items->currentIndex();
    auto *model = table_in_dialog->model();
    QString workspace_bash = model->data(model->index(row, 1)).toString() + "/devel/setup.bash";
    QString launch_file = model->data(model->index(row, 2)).toString();

    getTopicsOfTheLaunchFile(workspace_bash, launch_file);
}

void LaunchWidget::onRoslaunchFail(bool reset_toggle, const QString &err_msg) {
    progress_open->setVisible(false);
    toggle_start->setEnabled(true);
    checkbox_hz->setEnabled(false);
    combo_launch_items->setEnabled(true);
    combo_topic->setEnabled(false);
    label_hz->setText("");

    if(reset_toggle) {
        toggle_start->disconnect(SIGNAL(toggled(bool)),this, SLOT(onToggled(bool)));
        toggle_start->toggle();
        connect(toggle_start, SIGNAL(toggled(bool)),this, SLOT(onToggled(bool)));
    }

    animation_unactivate->start();
}

void LaunchWidget::detectRoslaunchResult() {
    timer_roslaunch_detect.stop();

    if(utils::getSystemProcessID(nullptr, "roslaunch", QString::number(process_launch->pid())).empty()) {
        onRoslaunchFail(true, "");
    }
    else {
        onRoslaunchSuccess();
    }
}

void LaunchWidget::updateLaunchCombo() {
    QStringList &&all_sensors = table_in_dialog->getAllKeys();
    combo_launch_items->clear();
    combo_launch_items->addItems(all_sensors);
}

void LaunchWidget::onTopicChanged(const QString &text) {
    if(text.isEmpty() || cur_monitored_topic == text)
        return;

    if(checkbox_hz->isChecked()) {
        stopMonitorTopicHz();
        monitorTopicHz(text);
    }
}

void LaunchWidget::onHzChecked(bool tog) {
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

void LaunchWidget::getTopicsOfTheLaunchFile(const QString &workspace_bash, const QString &file) {
    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    QProcess *p = pool.getOneProcess();

    QString source_cmd;
    if(roscore_widget->getSourceROSCmd(source_cmd)) {
        QStringList arguments;
        QString cmd = source_cmd;
        cmd += "source " + workspace_bash + ";";
        cmd += "roslaunch --nodes " + file + ";";
        arguments << "-c" << cmd;
        p->setArguments(arguments);
        p->start();
        p->waitForFinished();

        QString out_str;
        utils::getQProcessStandardOutput(p, out_str, true);
        QStringList node_list = out_str.split("\n");

        QStringList topic_list;
        for(auto &node : node_list) {
            cmd = source_cmd;
            cmd += "source " + workspace_bash + ";";
            cmd += "rosnode info -q " + node + ";";
            arguments.clear();
            arguments << "-c" << cmd;
            p->setArguments(arguments);
            p->start();
            p->waitForFinished();

            utils::getQProcessStandardOutput(p, out_str, true);
            QStringList pub_topic_info = utils::extractStringBetweenString(out_str, "Publications: \n", "\nSubscriptions: \n");
            if(pub_topic_info.isEmpty())
                continue;

            QStringList tmp_list = utils::extractAllByReg(QRegExp("(?:\\* )([^\\[ ]*)"), pub_topic_info.front());
            for(auto &topic: tmp_list) {
                if(!topic_list.contains(topic)) {
                    topic_list << topic;
                }
            }
        }

        topic_list.sort(Qt::CaseInsensitive);
        topic_list.push_front("");
        combo_topic->clear();
        combo_topic->addItems(topic_list);
    }

    pool.returnOneProcess(p);
}

void LaunchWidget::monitorTopicHz(const QString &topic) {
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

void LaunchWidget::stopMonitorTopicHz() {
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

void LaunchWidget::onHzOutput() {
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

void LaunchWidget::handleRoslaunchError() {
    QString out_str;
    utils::getQProcessStandardError(process_launch, out_str, true);

    if(!out_str.isEmpty()) {
        badge->appendMsg(out_str);
    }
}

void LaunchWidget::updateTopicCombo() {
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


