#include "grouplaunchwidget.h"
#include <iostream>

GroupLaunchWidget::GroupLaunchWidget(QWidget *parent, const QString &group_name, int allowed_module) : QWidget(parent), allowed_module_(allowed_module)
{
    label_name_ = new QLabel(this);
    label_name_->setText(group_name);
    label_name_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label_name_->setStyleSheet("background:transparent;border-width:0;border-style:outset;"
                               "font-size: 25px;"
                               "font-weight: bold;"
                               "color: #FF6633;"
                               );

    add_button_ = new QtMaterialRaisedButton(this);
    add_button_->setText("+");

    main_layout_ = new QGridLayout();
    main_layout_->setMargin(0);
    main_layout_->setSpacing(0);
    main_layout_->addWidget(label_name_, 0, 0, 1, 1, Qt::AlignLeft);

    group_layout_ = new QHBoxLayout();
    group_layout_->addWidget(add_button_, 0, Qt::AlignVCenter);

    main_layout_->addLayout(group_layout_, 1, 0, 1, 1, Qt::AlignTop);
    main_layout_->setRowStretch(0, 0);
    main_layout_->setRowStretch(1, 1);

    this->setLayout(main_layout_);

    dialog_ = new QDialog(this);
    dialog_->setWindowTitle("New Module");

    int row_d = 0;
    layout_dialog_ = new QGridLayout();

    label_module_in_dialog_ = new QLabel();
    label_module_in_dialog_->setText("Module");
    combo_module_in_dialog_ = new QComboBox();

    layout_dialog_->addWidget(label_module_in_dialog_, row_d, 0);
    layout_dialog_->addWidget(combo_module_in_dialog_, row_d, 1);
    row_d++;

    if(allowed_module_ & MODULE_SENSOR) {
        combo_module_in_dialog_->addItem("Sensor");

        label_type_in_dialog_ = new QLabel();
        label_type_in_dialog_->setText("Type");

        combo_type_in_dialog_ = new QComboBox();
        combo_type_in_dialog_->addItem("LiDAR");
        combo_type_in_dialog_->addItem("Camera");
        combo_type_in_dialog_->addItem("IMU");
        combo_type_in_dialog_->addItem("Others");

        layout_dialog_->addWidget(label_type_in_dialog_, row_d, 0);
        layout_dialog_->addWidget(combo_type_in_dialog_, row_d, 1);
        row_d++;
    }
    else{
        label_type_in_dialog_ = nullptr;
        combo_type_in_dialog_ = nullptr;
    }

    if(allowed_module_ & MODULE_SLAM) {
        combo_module_in_dialog_->addItem("SLAM");
    }
    if(allowed_module_ & MODULE_TOOL) {
        combo_module_in_dialog_->addItem("Tool");
    }

    label_name_in_dialog_ = new QLabel(dialog_);
    label_name_in_dialog_->setText("Name");
    edit_in_dialog_ =  new QLineEdit();
    layout_dialog_->addWidget(label_name_in_dialog_, row_d, 0);
    layout_dialog_->addWidget(edit_in_dialog_, row_d, 1);
    row_d++;

    button_in_dialog_ = new QtMaterialRaisedButton(dialog_);
    button_in_dialog_->setText("create");
    layout_dialog_->addWidget(button_in_dialog_, row_d, 0, 1, 2);
    row_d++;

    dialog_->setLayout(layout_dialog_);

    connect(button_in_dialog_, &QtMaterialRaisedButton::clicked, this, &GroupLaunchWidget::onButtonCreateClicked);
    connect(add_button_, &QtMaterialRaisedButton::clicked, this, &GroupLaunchWidget::onButtonAddClicked);
    connect(combo_module_in_dialog_, SIGNAL(currentTextChanged(QString)), this, SLOT(onModuleChanged(QString)));
}

void GroupLaunchWidget::appendWidget(LaunchWidget *wid) {
    if(wid != nullptr) {
        wid->setRemoveable(true);
        wid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        emit launchWidgetAdded(wid);
        connect(wid, SIGNAL(titleWidgetRemove(QWidget*)), this, SLOT(removeWidget(QWidget*)));

        int widget_num = group_layout_->count() - 1;
        group_layout_->insertWidget(widget_num, wid, 1);
    }
}

void GroupLaunchWidget::clearWidgets() {
    while(group_layout_->count() > 1) {
        QWidget *wid = group_layout_->itemAt(0)->widget();
        group_layout_->removeWidget(wid);
        wid->deleteLater();
    }
}

void GroupLaunchWidget::onButtonAddClicked() {
    LaunchWidget *wid_to_add = createAddedWidget();
    if(wid_to_add) {
        appendWidget(wid_to_add);
    }
}

void GroupLaunchWidget::onButtonCreateClicked() {
    dialog_->close();
    to_create_widget_  = true;
}

void GroupLaunchWidget::onModuleChanged(const QString &module_name) {
    if(module_name == "Sensor") {
        label_type_in_dialog_->show();
        combo_type_in_dialog_->show();
    }
    else if(label_type_in_dialog_ != nullptr) {
        label_type_in_dialog_->hide();
        combo_type_in_dialog_->hide();
    }
}

LaunchWidget* GroupLaunchWidget::createAddedWidget() {
    dialog_->exec();
    if(to_create_widget_) {
        to_create_widget_ = false;

        QString module_name = combo_module_in_dialog_->currentText();
        if(module_name == "Sensor"){
            return new SensorWidget(this, SensorType(combo_type_in_dialog_->currentIndex()), edit_in_dialog_->text());
        }
        else if(module_name == "SLAM") {
            return new SlamWidget(this);
        }
        else if(module_name == "Tool"){
            return new SensorWidget(this, SensorType::TOOLS, edit_in_dialog_->text());
        }
        else {
            return nullptr;
        }
    }
    else {
        return nullptr;
    }

}

void GroupLaunchWidget::removeWidget(QWidget* w) {
//    LaunchWidget *lw = static_cast<LaunchWidget*>(w);

//    if(lw->isLaunchProgramRunning()) {
//        auto ret = QMessageBox::question(this, "Remove Confirmation", "The program is stiil running, whether to remove?");
//        if(ret != QMessageBox::Yes)
//            return;
//    }

//    lw->prepareDisappearAnimation(20);

//    group_layout_->update();

    emit launchWidgetRemoved(static_cast<LaunchWidget*>(w));
    group_layout_->removeWidget(w);
    w->deleteLater();
}
