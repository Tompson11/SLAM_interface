#ifndef LAUNCHWIDGET_H
#define LAUNCHWIDGET_H

#include <chrono>
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
#include <QTreeView>
#include <QGraphicsDropShadowEffect>
#include "components/qtmaterialtextfield.h"
#include "components/qtmaterialraisedbutton.h"
#include "components/qtmaterialtoggle.h"
#include "components/qtmaterialsnackbar.h"
#include <unordered_map>
#include "components/qtmaterialcircularprogress.h"
#include "components/qtmaterialdialog.h"
#include "components/qtmaterialcheckbox.h"
#include "components/qtmaterialdrawer.h"
#include "components/lib/qtmaterialstyle.h"
#include "launchconfigdialog.h"
#include "roscorewidget.h"
#include "focusincombobox.h"
#include "launchtableview.h"
#include "titlewidget.h"
#include "errorbadgewidget.h"
#include "launchfilereader.h"
#include "launchparamdelegate.h"
#include "utils/sys.h"
#include "utils/shellpool.h"
#include "utils/reg.h"

class LaunchWidget : public TitleWidget
{
    Q_OBJECT

public:
    explicit LaunchWidget(QWidget *parent = nullptr, const QColor& unact_color = QColor(255,255,255), const QColor& act_color = QColor(255,255,255));
    ~LaunchWidget();

    bool isLaunchProgramRunning();

    void setRoscoreWidget(RoscoreWidget *ptr = nullptr);
    QAbstractItemModel* getTableModel();
    virtual void saveCurrentConfig(QSettings *settings, const QString &group, int index = 0) = 0;
    virtual void loadConfig(QSettings *settings, const QString &group, int index = 0) = 0;
    virtual void toggleCompactLayout();

protected:
    void registerToRoscore();
    void unregisterFromRoscore();

    void killRoslaunchProgram();

    void constructWidget();
    void constructLayout();
    void connectSignal();

    void getTopicsOfTheLaunchFile(const QString &workspace_bash, const QString &file);
    void monitorTopicHz(const QString &topic);
    void stopMonitorTopicHz();

    void setLaunchItemColor();
    void getLaunchRemapCmd(QString &cmd, QString launch_file);

    bool isRoscoreOpened();
    virtual void generateLaunchProgramName();

    bool use_compact_layout = false;

    // Widget
    QtMaterialCircularProgress *progress_open;

    QLabel *label_main_icon;

    QLabel *label_launch_items;
    FocusInComboBox *combo_launch_items;

    QLabel *label_topic;
    FocusInComboBox *combo_topic;

    QtMaterialCheckBox *checkbox_hz;
    QLabel *label_hz;

    QtMaterialRaisedButton *button_config;
    QtMaterialRaisedButton *button_log;

    LaunchConfigDialog *dialog_config;
    LaunchTableView *table_in_dialog;
    QtMaterialRaisedButton *button_add_in_dialog;
    QtMaterialRaisedButton *button_delete_in_dialog;

    QtMaterialDrawer *drawer_param;
    QTreeView *tree_param;
    QStandardItemModel *tree_model = nullptr;
    std::unordered_set<QStandardItem*> *changed_param_set = nullptr;
    std::unordered_set<QStandardItem*> *changed_arg_set = nullptr;

    typedef std::pair<QAbstractItemModel*, std::unordered_set<QStandardItem*>*> LaunchParamInfo;
    std::unordered_map<std::string, LaunchParamInfo> launch_file_map;

    // layout
    QGridLayout *drawer_layout;
    QGridLayout *dialog_layout;

    ErrorBadgeWidget *badge;

    // others useful
    QProcess *process_launch = nullptr;
    QProcess *process_topic = nullptr;

    QString cur_monitored_topic;

    QTimer timer_topic;
    QTimer timer_roslaunch_detect;

    RoscoreWidget *roscore_widget = nullptr;

    QString launch_program_name = "";

signals:

protected slots:
    virtual void onButtonRemoveClicked();
    void onToggled(bool tog);
    void onButtonAddClicked();
    void onButtonDeleteClicked();
    void onButtonConfigureClicked();
    void onButtonLogClicked();
    void onHzChecked(bool tog);
    void onTopicChanged(const QString &text);
    void onLaunchItemChanged(const QString &text);
    void onLaunchComboRightClicked();
    void onHzOutput();
    void onRoslaunchSuccess();
    void onRoslaunchFail(bool reset_toggle, const QString &err_msg);
    void updateTopicCombo();
    void updateLaunchCombo();
    void detectRoslaunchResult();
    void handleRoslaunchError();
    void handleRoslaunchStd();
    void onRoscoreClosed();
    void onLaunchParamChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());

};

#endif // LAUNCHWIDGET_H
