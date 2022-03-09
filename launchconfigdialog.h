#ifndef LAUNCHCONFIGDIALOG_H
#define LAUNCHCONFIGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QObject>
#include <QWidget>
#include <QCloseEvent>
#include "launchtableview.h"

class LaunchConfigDialog : public QDialog
{
    Q_OBJECT
public:
    LaunchConfigDialog(QWidget *parent = nullptr);
    void setTableView(LaunchTableView* table_view = nullptr);
    void closeEvent(QCloseEvent *event);

private:
    LaunchTableView *tableview;
    QMessageBox *message_box;
};

#endif // LAUNCHCONFIGDIALOG_H
