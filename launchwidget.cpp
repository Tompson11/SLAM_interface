#include "launchwidget.h"

LaunchWidget::LaunchWidget(QWidget *parent, const QColor& unact_color, const QColor& act_color) :
    TitleWidget(parent, unact_color, act_color)
{
    constructWidget();
    constructLayout();
    connectSignal();

}

LaunchWidget::~LaunchWidget() {
    killRoslaunchProgram();
    unregisterFromRoscore();
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

    combo_launch_items = new FocusInComboBox(this);

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

    drawer_param = new QtMaterialDrawer();
    drawer_param->setParent(this->parentWidget());
    drawer_param->setClickOutsideToClose(true);
    drawer_param->setOverlayMode(true);

    tree_param = new QTreeView();

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

    drawer_layout = new QGridLayout(drawer_param);
    drawer_layout->addWidget(tree_param, 0, 0, 1, 2);
    drawer_param->setDrawerLayout(drawer_layout);
}



void LaunchWidget::connectSignal() {
    connect(button_config, &QtMaterialRaisedButton::clicked, this, &LaunchWidget::onButtonConfigureClicked);
    connect(button_add_in_dialog, &QtMaterialRaisedButton::clicked, this, &LaunchWidget::onButtonAddClicked);
    connect(button_delete_in_dialog, &QtMaterialRaisedButton::clicked, this, &LaunchWidget::onButtonDeleteClicked);

    connect(toggle_start, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
    connect(checkbox_hz, SIGNAL(toggled(bool)), this, SLOT(onHzChecked(bool)));


    //    connect(combo_topic, SIGNAL(focusIn()), this, SLOT(updateTopicCombo()));
    connect(combo_topic, SIGNAL(currentTextChanged(QString)), this, SLOT(onTopicChanged(QString)));
    connect(combo_launch_items, SIGNAL(currentTextChanged(QString)), this, SLOT(onLaunchItemChanged(QString)));
    connect(combo_launch_items, SIGNAL(rightButtonPressed()), this, SLOT(onLaunchComboRightClicked()));

    connect(&timer_topic, &QTimer::timeout, this, &LaunchWidget::onHzOutput);
    connect(&timer_roslaunch_detect, &QTimer::timeout, this, &LaunchWidget::detectRoslaunchResult);

    connect(process_launch, &QProcess::readyReadStandardError,this,&LaunchWidget::handleRoslaunchError);
}

bool LaunchWidget::isLaunchProgramRunning() {
    return process_launch && process_launch->state() == QProcess::Running;
}


QAbstractItemModel* LaunchWidget::getTableModel() {
    return table_in_dialog->model();
}

void LaunchWidget::setRoscoreWidget(RoscoreWidget *ptr) {
    if(roscore_widget != nullptr) {
        roscore_widget->disconnect(this);
    }

    roscore_widget = ptr;

    if(roscore_widget != nullptr) {
        connect(roscore_widget, &RoscoreWidget::localRoscoreClosed, this, &LaunchWidget::onRoscoreClosed);
    }
}

void LaunchWidget::onRoscoreClosed() {
    if(toggle_start->isChecked()) {
        if(process_launch != nullptr && process_launch->state() == QProcess::Running) {
            utils::killAllChildProcess(nullptr, QString::number(process_launch->pid()));
        }

        stopMonitorTopicHz();
        onRoslaunchFail(true, "");
    }
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

void LaunchWidget::getLaunchRemapCmd(QString &cmd, QString launch_file) {
    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    auto *p = pool.getOneProcess();

    QString arg_override_cmd;
    QString arg_substitute_cmd;
    if(changed_arg_set != nullptr && changed_arg_set->empty() == false) {
        for(auto *item: *changed_arg_set) {
            QString name = tree_model->data(item->index().siblingAtColumn(1)).toString();
            QString type = tree_model->data(item->index().siblingAtColumn(2)).toString();
            QString value = tree_model->data(item->index().siblingAtColumn(3)).toString();

            if(type == "default") {
                arg_override_cmd += QString(" %1:=%2").arg(name, value);
            }
            else {
                QStringList tmp_arguments;
                QString tmp_cmd = QString("sed -n '/<arg.*name=\"%1\"/=' %2").arg(name, launch_file);
                tmp_arguments << "-c" << tmp_cmd;
                p->setArguments(tmp_arguments);
                p->start();
                p->waitForFinished();

                QString line;
                utils::getQProcessStandardOutput(p, line, true);

                arg_substitute_cmd += QString("| sed '%1 s/value=\".*\"/value=\"%2\"/' ").arg(line, value);
            }
        }
    }

    QString param_substitute_cmd;
    if(changed_param_set != nullptr && changed_param_set->empty() == false) {
        QString fake_params;
        for(auto *item: *changed_param_set) {
            QString ns = item->parent()->data(Qt::DisplayRole).toString();
            QString name = tree_model->data(item->index().siblingAtColumn(1)).toString();
            QString type = tree_model->data(item->index().siblingAtColumn(2)).toString();
            QString value = tree_model->data(item->index().siblingAtColumn(3)).toString();

            if(type.isEmpty())
                fake_params += QString("<param name=\"%1\" value=\"%2\" \\>\n").arg(name, value);
            else
                fake_params += QString("<param name=\"%1\" type=\"%2\" value=\"%3\" />\n").arg(name, type, value);

        }

        QStringList tmp_arguments;
        QString tmp_cmd = QString("sed -n '/<\\/launch>/=' %1").arg(launch_file);
        tmp_arguments << "-c" << tmp_cmd;
        p->setArguments(tmp_arguments);
        p->start();
        p->waitForFinished();

        QString line;
        utils::getQProcessStandardOutput(p, line, true);

        param_substitute_cmd = QString("| sed '%1 i\%2'").arg(line, fake_params);
    }

    cmd = "cat " + launch_file;
    cmd += arg_substitute_cmd;
    cmd += param_substitute_cmd;
    cmd += QString("| roslaunch - %1").arg(arg_override_cmd);

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
            if(!utils::existDir(launch_file)) {
                QMessageBox::critical(this, "non-existent launch file", launch_file + " is non-existent, please modify it first!");
                onRoslaunchFail(true, "");
                return;
            }

            QStringList arguments;

            QString tmp_cmd;
            getLaunchRemapCmd(tmp_cmd, launch_file);

            cmd += "source " + workspace_bash + ";";
            cmd += tmp_cmd;

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

        killRoslaunchProgram();

        onRoslaunchFail(false, "");
        unregisterFromRoscore();
    }
}

void LaunchWidget::generateLaunchProgramName() {
    int row = combo_launch_items->currentIndex();
    auto *model = table_in_dialog->model();
    QString time_stamp = QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    launch_program_name = model->data(model->index(row, 0)).toString() + "[" + time_stamp +"]";
}

void LaunchWidget::registerToRoscore() {
    if(roscore_widget) {
        generateLaunchProgramName();
        roscore_widget->registerRosProgram(launch_program_name);
    }
}

void LaunchWidget::unregisterFromRoscore() {
    if(roscore_widget)
        roscore_widget->unregisterRosProgram(launch_program_name);
}

void LaunchWidget::killRoslaunchProgram() {
    if(isLaunchProgramRunning()) {
        utils::killSystemProcess(nullptr, "roslaunch", QString::number(process_launch->pid()));
        process_launch->terminate();
        process_launch->waitForFinished();
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

    registerToRoscore();

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

void LaunchWidget::onLaunchItemChanged(const QString &text) {
    if(text.isEmpty()) {
        tree_model = nullptr;
        changed_param_set = nullptr;
        changed_arg_set = nullptr;
        setLaunchItemColor();
        return;
    }

    int row = combo_launch_items->currentIndex();
    auto *model = table_in_dialog->model();
    QString launch_file = model->data(model->index(row, 2)).toString();

    if(launch_file_map.count(launch_file.toStdString()) == 0) {
        tree_model = nullptr;
        changed_param_set = nullptr;
        changed_arg_set = nullptr;
    }
    else {
        auto &p = launch_file_map.at(launch_file.toStdString());
        tree_model = static_cast<QStandardItemModel*>(p.first);
        changed_param_set = p.second;
        changed_arg_set = p.second + 1;
    }
    setLaunchItemColor();

}

void LaunchWidget::onTopicChanged(const QString &text) {
    combo_topic->setToolTip(text);

    if(text.isEmpty() || cur_monitored_topic == text)
        return;

    if(checkbox_hz->isChecked()) {
        stopMonitorTopicHz();
        monitorTopicHz(text);
    }
}

void LaunchWidget::onLaunchParamChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
    if(bottomRight.column() < 3)
        return;

    for(int row = topLeft.row() ; row <= bottomRight.row(); row++) {
        auto *item = tree_model->itemFromIndex(topLeft.sibling(row, 3));
        item->setToolTip(item->data(Qt::DisplayRole).toString());

        auto *arg_header_item = tree_model->item(0);
        auto *param_header_item = tree_model->item(1);

        if(item->data(Qt::UserRole) != item->data(Qt::DisplayRole)) {
            item->setForeground(QBrush(QColor(255, 0, 0)));

            if(item->parent() == arg_header_item)
                changed_arg_set->emplace(item);
            else if(item->parent()->parent() == param_header_item)
                changed_param_set->emplace(item);

            setLaunchItemColor();
        }
        else {
            item->setForeground(QBrush(QColor(0, 0, 0)));

            if(item->parent() == arg_header_item)
                changed_arg_set->erase(item);
            else if(item->parent() != nullptr && item->parent()->parent() == param_header_item)
                changed_param_set->erase(item);

            setLaunchItemColor();
        }
    }
}

void LaunchWidget::onLaunchComboRightClicked() {
    int row = combo_launch_items->currentIndex();
    auto *model = table_in_dialog->model();
    QString launch_file = model->data(model->index(row, 2)).toString();

    if(!utils::existDir(launch_file))
        return;

    if(launch_file_map.count(launch_file.toStdString()) == 0) {

        tree_model = new QStandardItemModel();
        tree_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("Namespace")<<QStringLiteral("Parameter") << QStringLiteral("Type") << QStringLiteral("Value") << QStringLiteral(""));

        LaunchParamDelegate *delegate = new LaunchParamDelegate(this, tree_model);
        tree_param->setItemDelegate(delegate);

        LaunchFileReader reader(launch_file, tree_model);
        QString err_msg;
        if(reader.readAndBuildModel(err_msg)) {
            std::unordered_set<QStandardItem*> *set_array = new std::unordered_set<QStandardItem*>[2];
            changed_param_set = &set_array[0];
            changed_arg_set = &set_array[1];
            launch_file_map.emplace(launch_file.toStdString(), LaunchParamInfo(tree_model, set_array));
            connect(tree_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onLaunchParamChanged(QModelIndex,QModelIndex,QVector<int>)));
        }
        else {
            delete tree_model;
            tree_model = nullptr;
            changed_param_set = nullptr;
            changed_arg_set = nullptr;
            return;
        }
    }

    tree_param->setModel(tree_model);
    tree_param->setColumnWidth(0, 100);
    tree_param->setColumnWidth(1, 200);
    tree_param->setColumnWidth(2, 100);
    tree_param->setColumnWidth(3, 100);
    tree_param->setColumnWidth(4, 50);

    drawer_param->openDrawer();
}

void LaunchWidget::onHzChecked(bool tog) {
    if(tog) {
        if(!combo_topic->currentText().isEmpty()) {
            monitorTopicHz(combo_topic->currentText());
        }
    }
    else {
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
        QString cur_text = combo_topic->currentText();
        combo_topic->clear();
        combo_topic->addItems(topic_list);
        combo_topic->setCurrentText(cur_text);
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

void LaunchWidget::setLaunchItemColor() {
    if((changed_param_set == nullptr || changed_param_set->empty()) && (changed_arg_set == nullptr || changed_arg_set->empty()))
        label_launch_items->setStyleSheet("color: #000000;");
    else
        label_launch_items->setStyleSheet("color: #FF0000;");

}

void LaunchWidget::onButtonRemoveClicked() {
    if(isLaunchProgramRunning()) {
        auto ret = QMessageBox::question(this, "Remove Confirmation", "The program is stiil running, whether to remove?");
        if(ret != QMessageBox::Yes)
            return;
    }

    this->prepareDisappearAnimation(30);
}
