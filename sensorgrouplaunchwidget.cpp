#include "sensorgrouplaunchwidget.h"
#include <iostream>

SensorGroupLaunchWidget::SensorGroupLaunchWidget(QWidget *parent) : GroupLaunchWidget(parent)
{
    dialog_ = new QDialog(this);
    dialog_->setWindowTitle("New Sensor");

    label_name_in_dialog_ = new QLabel();
    label_name_in_dialog_->setText("Name");

    label_type_in_dialog_ = new QLabel();
    label_type_in_dialog_->setText("Type");

    edit_in_dialog_ =  new QLineEdit();

    combo_in_dialog_ = new QComboBox();
    combo_in_dialog_->addItem("LIDAR");
    combo_in_dialog_->addItem("CAMERA");
    combo_in_dialog_->addItem("IMU");
    combo_in_dialog_->addItem("OTHERS");

    button_in_dialog_ = new QtMaterialRaisedButton();
    button_in_dialog_->setText("create");

    layout_dialog_ = new QGridLayout();
    layout_dialog_->addWidget(label_type_in_dialog_, 0, 0);
    layout_dialog_->addWidget(combo_in_dialog_, 0, 1);
    layout_dialog_->addWidget(label_name_in_dialog_, 1, 0);
    layout_dialog_->addWidget(edit_in_dialog_, 1, 1);
    layout_dialog_->addWidget(button_in_dialog_, 2, 0, 1, 2);

    dialog_->setLayout(layout_dialog_);

    connect(button_in_dialog_, &QtMaterialRaisedButton::clicked, this, &SensorGroupLaunchWidget::onButtonCreateClicked);
}

LaunchWidget* SensorGroupLaunchWidget::createAddedWidget() {
    dialog_->exec();
    LaunchWidget* wid_to_add = new SensorWidget(this, SensorType(combo_in_dialog_->currentIndex()), edit_in_dialog_->text());
    return wid_to_add;
}

void SensorGroupLaunchWidget::onButtonCreateClicked() {
    dialog_->close();
}

