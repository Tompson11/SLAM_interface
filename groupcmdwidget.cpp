#include "groupcmdwidget.h"
#include <QFontMetrics>
GroupCmdWidget::GroupCmdWidget(QWidget *parent) : QWidget(parent)
{
    label_title_ = new QLabel(this);
    label_title_->setAlignment(Qt::AlignCenter);
    label_title_->setStyleSheet("background:transparent;border-width:0;border-style:outset;"
                               "font-size: 20px;"
                               "font-weight: bold;"
                               );
    label_title_->setText("   Command   ");
    label_title_->setMinimumHeight(45);

    label_title_back_ = new QLabel(this);
    label_title_back_->setMinimumHeight(24);
    label_title_back_->setStyleSheet("background: #E6BEFF;border:2px solid #E6BEFF;");

    list_widget_ = new QListWidget(this);
    list_widget_->setFlow(QListView::LeftToRight);
    list_widget_->setResizeMode(QListView::Adjust);
    list_widget_->setViewMode(QListView::IconMode);
    list_widget_->setSelectionMode(QAbstractItemView::NoSelection);

    button_add_ = new QtMaterialRaisedButton(this);
    button_add_->setBackgroundColor(QtMaterialStyle::instance().themeColor("accent1"));
    button_add_->setText("+");

    QFrame *button_frame = new QFrame(this);
    QGridLayout *button_layout = new QGridLayout();
    button_layout->addWidget(button_add_);
    button_frame->setLayout(button_layout);

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(button_frame->sizeHint());
    list_widget_->addItem(item);
    list_widget_->setItemWidget(item, button_frame);
    list_widget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout_ = new QGridLayout();
    layout_->addWidget(label_title_back_, 0, 0, 1, 1);
    layout_->addWidget(label_title_, 0, 0, 1, 1, Qt::AlignCenter);
    layout_->addWidget(list_widget_, 0, 1, 1, 1);
    layout_->setSpacing(0);
    layout_->setMargin(0);
    layout_->setColumnStretch(0, 0);
    layout_->setColumnStretch(1, 1);

    frame_ = new QFrame();
    frame_->setLayout(layout_);

    layout_main_ = new QGridLayout();
    layout_main_->addWidget(frame_);
    this->setLayout(layout_main_);

    connect(button_add_, &QtMaterialRaisedButton::clicked, this, &GroupCmdWidget::onButtonAddClicked);
}

void GroupCmdWidget::toggleCompactLayout() {
    use_compact_layout_ = !use_compact_layout_;

    if(use_compact_layout_) {
        layout_->addWidget(list_widget_, 1, 0, 1, 1);
        layout_->setRowStretch(0, 0);
        layout_->setRowStretch(1, 1);
    }
    else {
        layout_->addWidget(list_widget_, 0, 1, 1, 1);
        layout_->setColumnStretch(0, 0);
        layout_->setColumnStretch(1, 1);
    }
}

void GroupCmdWidget::addCmdWidget(const QString &cmd_name, const QString &cmd_code) {
    QListWidgetItem *item = new QListWidgetItem();
    CmdWidget *new_widget = new CmdWidget();
    new_widget->setCmdName(cmd_name);
    new_widget->setCmdCode(cmd_code);

    item->setSizeHint(new_widget->sizeHint());
    list_widget_->insertItem(list_widget_->model()->rowCount() - 1, item);
    list_widget_->setItemWidget(item, new_widget);

    m_cmd_to_list_.emplace(new_widget, item);
    onCmdWidgetResized(new_widget);

    connect(new_widget, SIGNAL(resized(CmdWidget*)), this, SLOT(onCmdWidgetResized(CmdWidget*)));
    connect(new_widget, SIGNAL(deleted(CmdWidget*)), this, SLOT(onCmdWidgetDeleted(CmdWidget*)));
}


void GroupCmdWidget::onButtonAddClicked() {
    addCmdWidget("", "");
}

void GroupCmdWidget::onCmdWidgetResized(CmdWidget *cmd_wid) {
    m_cmd_to_list_[cmd_wid]->setSizeHint(cmd_wid->sizeHint());
    list_widget_->setFrameShape(QFrame::NoFrame);
    list_widget_->setFrameShape(QFrame::Box);
}

void GroupCmdWidget::onCmdWidgetDeleted(CmdWidget *cmd_wid) {
    QListWidgetItem *list_wid = m_cmd_to_list_[cmd_wid];
    list_widget_->model()->removeRow(list_widget_->row(list_wid));

    m_cmd_to_list_.erase(cmd_wid);
    cmd_wid->deleteLater();
}

void GroupCmdWidget::saveCurrentConfig(QSettings *settings, const QString &group) {
    if(settings) {
        settings->beginGroup(group);
        settings->beginWriteArray("CMD");
        auto* model = list_widget_->model();
        for (int i = 0; i < model->rowCount() - 1; i++) {
            settings->setArrayIndex(i);
            CmdWidget *cmd_wid = static_cast<CmdWidget*>(list_widget_->itemWidget(list_widget_->item(i)));
            settings->setValue("NAME", cmd_wid->getCmdName());
            settings->setValue("CODE", cmd_wid->getCmdCode());
        }
        settings->endArray();
        settings->endGroup();
    }
}

void GroupCmdWidget::loadConfig(QSettings *settings, const QString &group) {
    if(settings) {
        settings->beginGroup(group);

        int size = settings->beginReadArray("CMD");
        for(int i = 0; i < size; i++) {
            settings->setArrayIndex(i);
            addCmdWidget(settings->value("NAME").toString(), settings->value("CODE").toString());
        }

        settings->endArray();
        settings->endGroup();
    }
}
