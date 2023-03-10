#include "cmdwidget.h"
#include <iostream>
CmdWidget::CmdWidget(QWidget *parent) : QWidget(parent)
{
    button_cmd_ = new QtMaterialRaisedButton(this);

    button_delete_ = new QtMaterialRaisedButton(this);
    button_delete_->setText("x");
    button_delete_->setBackgroundColor(QtMaterialStyle::instance().themeColor("accent1"));
    button_delete_->setFixedHeight(15);
    button_delete_->setFixedWidth(15);
    button_delete_->hide();

    layout_main_ = new QGridLayout();
    layout_main_->addWidget(button_cmd_, 0, 0, 1, 1);
    layout_main_->addWidget(button_delete_, 0, 0, 1, 1, Qt::AlignRight | Qt::AlignTop);
    layout_main_->setSpacing(0);
    this->setLayout(layout_main_);

    lineedit_in_dialog_rename_ = new QLineEdit();

    layout_dialog_rename_ = new QGridLayout();
    layout_dialog_rename_->addWidget(lineedit_in_dialog_rename_);

    dialog_rename_ = new QDialog();
    dialog_rename_->setWindowTitle("Rename the commond");
    dialog_rename_->setLayout(layout_dialog_rename_);

    textedit_in_dialog_cmd_ = new QTextEdit();

    button_clear_in_dialog_cmd_ = new QtMaterialRaisedButton();
    button_clear_in_dialog_cmd_->setText("clear");

    checkbox_in_dialog_cmd_ = new QtMaterialCheckBox();
    checkbox_in_dialog_cmd_->setText("wait on exit");

    layout_dialog_cmd_ = new QGridLayout();
    layout_dialog_cmd_->addWidget(textedit_in_dialog_cmd_, 0, 0, 3, 3);
    layout_dialog_cmd_->addWidget(button_clear_in_dialog_cmd_, 0, 3, 1, 1);
    layout_dialog_cmd_->addWidget(checkbox_in_dialog_cmd_, 1, 3, 1, 1);

    dialog_cmd_ = new QDialog();
    dialog_cmd_->setWindowTitle("Edit the commond");
    dialog_cmd_->setLayout(layout_dialog_cmd_);

    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    process_cmd_ = pool.getOneProcess();

    connect(button_cmd_, &QtMaterialRaisedButton::clicked, this, &CmdWidget::onButtonCmdClicked);
    connect(button_delete_, &QtMaterialRaisedButton::clicked, this, &CmdWidget::onButtonDeleteClicked);
    connect(button_clear_in_dialog_cmd_, &QtMaterialRaisedButton::clicked, this, &CmdWidget::onButtonClearClicked);
    connect(checkbox_in_dialog_cmd_, SIGNAL(toggled(bool)), this, SLOT(onChecked(bool)));
    connect(lineedit_in_dialog_rename_, SIGNAL(returnPressed()), this, SLOT(onRenameReturned()));

    this->setMouseTracking(true);
    this->setAcceptDrops(true);
}

void CmdWidget::setColor(bool is_clicked) {
    button_cmd_->setBackgroundColor(is_clicked ? "#00CCCC" : "#000000");
}

void CmdWidget::updateButtonCmdSize() {
    button_cmd_->setMinimumWidth(button_cmd_->fontMetrics().width(button_cmd_->text()) + 6);
}

void CmdWidget::setRoscoreWidget(RoscoreWidget *wid) {
    roscore_widget = wid;
}


void CmdWidget::setCmdName(const QString &name) {
    button_cmd_->setText(name);
    updateButtonCmdSize();
}

void CmdWidget::setCmdCode(const QString &cmd) {
    textedit_in_dialog_cmd_->setText(cmd);
}

QString CmdWidget::getCmdName() {
    return button_cmd_->text();
}

QString CmdWidget::getCmdCode() {
    return textedit_in_dialog_cmd_->toPlainText();
}

void CmdWidget::mousePressEvent(QMouseEvent *event) {
    if(button_cmd_->geometry().contains(event->pos())) {
        if(event->button() == Qt::MidButton) {
            lineedit_in_dialog_rename_->setText(button_cmd_->text());
            dialog_rename_->setGeometry(event->globalX(), event->globalY(), 0, 0);
            dialog_rename_->resize(250, 0);
            dialog_rename_->exec();

            setCmdName(lineedit_in_dialog_rename_->text());
            emit resized(this);
        }
        else if(event->button() == Qt::RightButton){
            dialog_cmd_->setGeometry(event->globalX(), event->globalY(), 0, 0);
            dialog_cmd_->resize(350, 100);
            dialog_cmd_->exec();
        }
    }


}

void CmdWidget::enterEvent(QEvent *event) {
    button_delete_->show();
}

void CmdWidget::leaveEvent(QEvent *event) {
    button_delete_->hide();
}

void CmdWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
            return;
//    if (!button_cmd_->rect().contains(event->pos()))
//            return;

    emit draged();
    this->setColor(true);

    QDrag *drag = new QDrag(this);
    QMimeData *data = new QMimeData();
    data->setText(QString::number(reinterpret_cast<qulonglong>(this)));
    drag->setMimeData(data);
    drag->exec(Qt::MoveAction);

    this->setColor(false);
};

void CmdWidget::dragEnterEvent(QDragEnterEvent *event) {
    if(event->mimeData()->hasText()) {
        event->acceptProposedAction();
        this->setColor(true);

        CmdWidget *drag_wid_ptr = reinterpret_cast<CmdWidget*>(event->mimeData()->text().toULongLong());
        emit cmdWidgetChangePosition(drag_wid_ptr, this);
    }
}

void CmdWidget::dragLeaveEvent(QDragLeaveEvent *event) {
//    event->accept();
    this->setColor(false);
}

void CmdWidget::dropEvent(QDropEvent *event) {
    event->acceptProposedAction();
    this->setColor(false);
}

void CmdWidget::onButtonCmdClicked() {
    QStringList arguments;
    QString source_cmd = "";
    if(roscore_widget != nullptr)
        roscore_widget->getSourceROSCmd(source_cmd);
    QString cmd = textedit_in_dialog_cmd_->toPlainText();
    QString total_cmd = QString("gnome-terminal --title '%1' -- bash -c '%2%3'").arg(button_cmd_->text()).arg(source_cmd).arg(cmd);
    arguments << "-c" << total_cmd;

    process_cmd_->setArguments(arguments);
    process_cmd_->start();
}

void CmdWidget::onButtonDeleteClicked() {
    emit deleted(this);
}

void CmdWidget::onButtonClearClicked() {
    textedit_in_dialog_cmd_->clear();
}

void CmdWidget::onChecked(bool tog) {
    QString old_cmd = textedit_in_dialog_cmd_->toPlainText();
    if(tog) {
        QString append_cmd = old_cmd.endsWith(";") ? "read;" : ";read;";
        textedit_in_dialog_cmd_->setText(old_cmd + append_cmd);
    }
    else if(old_cmd.endsWith("read;")){
        textedit_in_dialog_cmd_->setText(old_cmd.mid(0, old_cmd.size() - 5));
    }
    else if(old_cmd.endsWith("read")){
        textedit_in_dialog_cmd_->setText(old_cmd.mid(0, old_cmd.size() - 4));
    }
}

void CmdWidget::onRenameReturned() {
    dialog_rename_->close();
}

void CmdWidget::prepareForChangingPosition() {
    wid_hold_my_data = this;
    wid_i_hold_its_data = this;
}

CmdWidget *CmdWidget::getWidHoldMyData() {
    return this->wid_hold_my_data;
}

void CmdWidget::swapDisplay(CmdWidget *src, CmdWidget *target) {
    QString cmd_name = target->button_cmd_->text();
    QString cmd_code = target->textedit_in_dialog_cmd_->toPlainText();
    bool checked = target->checkbox_in_dialog_cmd_->isChecked();

    target->button_cmd_->setText(src->button_cmd_->text());
    target->checkbox_in_dialog_cmd_->setChecked(src->checkbox_in_dialog_cmd_->isChecked());
    target->textedit_in_dialog_cmd_->setText(src->textedit_in_dialog_cmd_->toPlainText());

    src->button_cmd_->setText(cmd_name);
    src->checkbox_in_dialog_cmd_->setChecked(checked);
    src->textedit_in_dialog_cmd_->setText(cmd_code);

    std::swap(target->wid_i_hold_its_data, src->wid_i_hold_its_data);
    src->wid_i_hold_its_data->wid_hold_my_data = src;
    target->wid_i_hold_its_data->wid_hold_my_data = target;
}

