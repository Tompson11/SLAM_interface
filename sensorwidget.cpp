#include "sensorwidget.h"

SensorWidget::SensorWidget(QWidget *parent) : QWidget(parent)
{
    label_sensor_icon = new QLabel(this);
    label_sensor_icon->setText("sensor icon");

    toggle_start = new QtMaterialToggle(this);

    combo_sensor = new QComboBox(this);

    label_hz = new QLabel(this);

    button_config = new QtMaterialRaisedButton(this);
    button_config->setText("Configure");

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(label_sensor_icon, 0, 0, 1, 1);
    layout->addWidget(toggle_start, 1, 0, 1, 1);
    layout->addWidget(combo_sensor, 2, 0, 1, 1);
    layout->addWidget(label_hz, 3, 0 ,1, 1);
    layout->addWidget(button_config, 4, 0 ,1, 1);
    this->setLayout(layout);

    table_in_dialog = new LaunchTableView(dialog_config);
    button_add_in_dialog = new QtMaterialRaisedButton(dialog_config);
    button_add_in_dialog->setText("Add");

    dialog_config = new LaunchConfigDialog(this, table_in_dialog);

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

    connect(table_in_dialog, SIGNAL(launchTableUpdate()), this, SLOT(updateCombo()));
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

void SensorWidget::updateCombo() {
    QStringList &&all_sensors = table_in_dialog->getAllKeys();
    combo_sensor->clear();
    combo_sensor->addItems(all_sensors);
}
