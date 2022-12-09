#include "grouplaunchwidget.h"
#include <iostream>

GroupLaunchWidget::GroupLaunchWidget(QWidget *parent, const QString &group_name, int allowed_module) : QWidget(parent), allowed_module_(allowed_module)
{
    add_button_ = new QtMaterialRaisedButton(this);
    add_button_->setText("+");

    main_layout_ = new QGridLayout();
    main_layout_->setMargin(0);
    main_layout_->setSpacing(0);

    group_layout_ = new QHBoxLayout();
    group_layout_->addWidget(add_button_, 0, Qt::AlignVCenter);

    group_layout_compact_ = new QVBoxLayout();
    group_layout_compact_->addWidget(add_button_, 0, Qt::AlignTop);

    main_layout_->addLayout(group_layout_, 1, 0, 1, 1);
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

void GroupLaunchWidget::toggleCompactLayout() {
    use_compact_layout_ = !use_compact_layout_;

    if(use_compact_layout_) {
        main_layout_->addLayout(group_layout_compact_, 1, 0, 1, 1);
        main_layout_->removeItem(group_layout_);
    }
    else {
        main_layout_->addLayout(group_layout_, 1, 0, 1, 1);
        main_layout_->removeItem(group_layout_compact_);
    }
}

int GroupLaunchWidget::getWidgetIndex(QWidget *wid) {
    return group_layout_->indexOf(wid);
}

void GroupLaunchWidget::appendWidget(LaunchWidget *wid) {
    if(wid != nullptr) {
        wid->setRemoveable(true);
        wid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect(wid, SIGNAL(titleWidgetRemove(QWidget*)), this, SLOT(removeWidget(QWidget*)));
        connect(wid, SIGNAL(titleWidgetChangePosition(QWidget*, QWidget*)), this, SLOT(bubbleChangeWidgetPosition(QWidget*, QWidget*)));

        int widget_num = group_layout_->count() - 1;
        if(use_compact_layout_) {
            wid->toggleCompactLayout();
            group_layout_->insertWidget(widget_num, wid, 1);
            group_layout_compact_->insertWidget(widget_num, wid, 1, Qt::AlignTop);
        }
        else {
            group_layout_compact_->insertWidget(widget_num, wid, 1, Qt::AlignTop);
            group_layout_->insertWidget(widget_num, wid, 1);
        }

        emit launchWidgetAdded(wid);
    }
}

void GroupLaunchWidget::clearWidgets() {
    while(group_layout_->count() > 1) {
        QWidget *wid = group_layout_->itemAt(0)->widget();
        group_layout_->removeWidget(wid);
        group_layout_compact_->removeWidget(wid);
        wid->deleteLater();
    }
}

int GroupLaunchWidget::getCompactHeight() {
    return 66 * (group_layout_compact_->count() - 1) + 2 * group_layout_compact_->spacing() * group_layout_compact_->count() + add_button_->height() + 2 * group_layout_compact_->margin();
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
            return new SlamWidget(this, edit_in_dialog_->text());
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

    group_layout_->removeWidget(w);
    group_layout_compact_->removeWidget(w);
    emit launchWidgetRemoved(static_cast<LaunchWidget*>(w));
    w->deleteLater();
}

void GroupLaunchWidget::bubbleChangeWidgetPosition(QWidget* w_from, QWidget* w_to) {
    int id_from = group_layout_->indexOf(w_from);
    int id_to = group_layout_->indexOf(w_to);

    if(id_from < 0 || id_to < 0)
        return;

    if(!use_compact_layout_) {
        group_layout_compact_->removeWidget(w_from);
        group_layout_compact_->insertWidget(id_to, w_from);

        group_layout_->removeWidget(w_from);
        group_layout_->insertWidget(id_to, w_from);
    }
    else {
        group_layout_->removeWidget(w_from);
        group_layout_->insertWidget(id_to, w_from);

        group_layout_compact_->removeWidget(w_from);
        group_layout_compact_->insertWidget(id_to, w_from);
    }
}
