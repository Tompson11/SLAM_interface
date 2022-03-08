#include "launchconfigdialog.h"
#include <iostream>
LaunchConfigDialog::LaunchConfigDialog(QWidget *parent, LaunchTableView *table_view): QDialog(parent), tableview(table_view)
{
    message_box = new QMessageBox(this);
    message_box->setInformativeText("Do you want to delete them?");
    message_box->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message_box->setDefaultButton(QMessageBox::No);
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
