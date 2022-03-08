#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QObject>
#include <QWidget>
#include <QProcess>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QTimer>
#include <QFileDialog>
#include <QComboBox>
#include "components/qtmaterialtextfield.h"
#include "components/qtmaterialraisedbutton.h"
#include "components/qtmaterialtoggle.h"
#include "components/qtmaterialsnackbar.h"
#include <unordered_map>
#include "components/qtmaterialcircularprogress.h"
#include "components/qtmaterialdialog.h"
#include "launchtableview.h"
#include "launchconfigdialog.h"
#include "roscorewidget.h"
#include "utils/sys.h"
#include "utils/shellpool.h"

class SensorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SensorWidget(QWidget *parent = nullptr);
    void setRoscoreWidget(RoscoreWidget *ptr = nullptr);

//    void enterEvent(QEvent *e);
//    void leaveEvent(QEvent *e);

private:
    QLabel *label_sensor_icon;
    QtMaterialToggle *toggle_start;

    QComboBox *combo_sensor;
    QLabel *label_hz;

    QtMaterialRaisedButton *button_config;
    LaunchConfigDialog *dialog_config;

    LaunchTableView *table_in_dialog;
    QtMaterialRaisedButton *button_add_in_dialog;
    QtMaterialRaisedButton *button_delete_in_dialog;

    QProcess *process_launch;

    RoscoreWidget *roscore_widget;

signals:

private slots:
    void onToggled(bool tog);
    void onButtonAddClicked();
    void onButtonDeleteClicked();
    void onButtonConfigureClicked();
    void updateCombo();
};

#endif // SENSORWIDGET_H
