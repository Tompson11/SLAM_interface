#ifndef LAUNCHWIDGET_H
#define LAUNCHWIDGET_H

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
#include "components/qtmaterialcheckbox.h"
#include "launchconfigdialog.h"
#include "roscorewidget.h"
#include "focusincombobox.h"
#include "utils/sys.h"
#include "utils/shellpool.h"
#include "utils/reg.h"

class LaunchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LaunchWidget(QWidget *parent = nullptr);
    void setRoscoreWidget(RoscoreWidget *ptr = nullptr);
    virtual void saveCurrentConfig(QSettings *settings, const QString &group) = 0;
    virtual void loadConfig(QSettings *settings, const QString &group) = 0;

protected:
    void getTopicsOfTheLaunchFile(const QString &workspace_bash, const QString &file);
    void monitorTopicHz(const QString &topic);
    void stopMonitorTopicHz();

    QtMaterialCircularProgress *progress_open;

    QLabel *label_main_icon;
    QtMaterialToggle *toggle_start;

    QLabel *label_launch_items;
    QComboBox *combo_launch_items;

    QLabel *label_topic;
    FocusInComboBox *combo_topic;

    QtMaterialCheckBox *checkbox_hz;
    QLabel *label_hz;

    QtMaterialRaisedButton *button_config;

    LaunchConfigDialog *dialog_config;
    LaunchTableView *table_in_dialog;
    QtMaterialRaisedButton *button_add_in_dialog;
    QtMaterialRaisedButton *button_delete_in_dialog;

    QProcess *process_launch = nullptr;
    QProcess *process_topic = nullptr;

    QString cur_monitored_topic;

    QTimer timer_topic;
    QTimer timer_roslaunch_detect;

    RoscoreWidget *roscore_widget;

signals:

protected slots:
    void onToggled(bool tog);
    void onButtonAddClicked();
    void onButtonDeleteClicked();
    void onButtonConfigureClicked();
    void onHzChecked(bool tog);
    void onTopicChanged(const QString &text);
    void onHzOutput();
    void onRoslaunchSuccess();
    void onRoslaunchFail(bool reset_toggle, const QString &err_msg);
    void updateTopicCombo();
    void updateLaunchCombo();
    void detectRoslaunchResult();

};

#endif // LAUNCHWIDGET_H
