#include "errorbadgewidget.h"

ErrorBadgeWidget::ErrorBadgeWidget(QWidget *parent) : QWidget(parent)
{
    badge = new clickableBadge();
    badge->setVisible(false);

    dialog_msg = new QDialog(this);
    dialog_msg->setWindowTitle("Log");

    textedit_in_dialog = new QTextEdit(this);
    textedit_in_dialog->setReadOnly(true);

    button_clear_in_dialog = new QtMaterialRaisedButton(this);
    button_clear_in_dialog->setText("clear");

    layout_dialog = new QGridLayout();
    layout_dialog->addWidget(textedit_in_dialog, 0, 0, 1, 1);
    layout_dialog->addWidget(button_clear_in_dialog, 0, 1, 1, 1);

    dialog_msg->setLayout(layout_dialog);

    connect(badge, &clickableBadge::clicked, this, &ErrorBadgeWidget::onBadgeClicked);
    connect(button_clear_in_dialog, &QtMaterialRaisedButton::clicked, this, &ErrorBadgeWidget::onButtonClearClicked);
}

void ErrorBadgeWidget::setBadgeParent(QWidget *parent) {
    badge->setParent(parent);
}

void ErrorBadgeWidget::setBadgeShift(int shift_x, int shift_y) {
    badge->setRelativePosition(shift_x, shift_y);
}

void ErrorBadgeWidget::onBadgeClicked() {
    badge->setVisible(false);
    dialog_msg->exec();
    unread_msg_nbr = 0;
}

void ErrorBadgeWidget::onButtonClearClicked() {
    textedit_in_dialog->clear();
}

void ErrorBadgeWidget::appendMsg(QString msg) {
    msg.replace("\033[31m", MSG_RED);
    msg.replace("\033[32m", MSG_GREEN);
    msg.replace("\033[33m", MSG_YELLOW);
    msg.replace("\033[34m", MSG_BLUE);
    msg.replace("\033[35m", MSG_MAGENTA);
    msg.replace("\033[36m", MSG_CYAN);
    msg.replace("\033[0m","</font>");
    msg.replace("\n","<br>");

    textedit_in_dialog->append(msg.trimmed());
    textedit_in_dialog->update();
    unread_msg_nbr++;

    badge->setText(QString::number(unread_msg_nbr));
    badge->setVisible(true);
}
