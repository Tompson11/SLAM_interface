#include "launchconfigdialog.h"
#include <iostream>

LaunchConfigDialog::LaunchConfigDialog(QWidget *parent): QDialog(parent)
{
    message_box = new QMessageBox(this);
    message_box->setInformativeText("Do you want to delete them?");
    message_box->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message_box->setDefaultButton(QMessageBox::No);
}

void LaunchConfigDialog::setTableView(LaunchTableView* table_view) {
    this->tableview = table_view;
}

void LaunchConfigDialog::closeEvent(QCloseEvent *event) {
    if(tableview->existIncompleteRows()) {
        QString text = "Line " + tableview->getIncompleteRowsInQString(",") + " are incomplete!";
        message_box->setText(text);
        switch (message_box->exec()) {
        case QMessageBox::Yes: {
            tableview->clearIncompleteRows();
            event->accept();
            break;
        }
        case QMessageBox::No: {
            event->ignore();
            return;
        }
        }
    }

    if(tableview->key_changed) {
        tableview->notifyUpdate();
        tableview->key_changed = false;
    }
}
