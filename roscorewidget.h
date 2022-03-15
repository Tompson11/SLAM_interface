#ifndef ROSCOREWIDGET_H
#define ROSCOREWIDGET_H

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
#include <QSettings>
#include <QPixmap>
#include <mutex>
#include <unordered_set>
#include "titlewidget.h"
#include "errorbadgewidget.h"
#include "components/qtmaterialtextfield.h"
#include "components/qtmaterialraisedbutton.h"
#include "components/qtmaterialtoggle.h"
#include "components/qtmaterialsnackbar.h"
#include "components/qtmaterialcircularprogress.h"


#define ROS_DISTROS_NUM (8)

class RoscoreWidget : public TitleWidget
{
    Q_OBJECT
public:
    explicit RoscoreWidget(QWidget *parent = nullptr, const QColor& unact_color = QColor(255,255,255), const QColor& act_color = QColor(255,255,255));
    bool getSourceROSCmd(QString &cmd);
    void saveCurrentConfig(QSettings *settings, const QString &group);
    void loadConfig(QSettings *settings, const QString &group);
    bool isRoscoreOpened();
    void registerRosProgram(const QString &program);
    void unregisterRosProgram(const QString &program);
    ~RoscoreWidget();

private:
    void setMasterURI(const QString &uri);
    bool validateRosPath(const QString &path, bool pop_error = false);
    void detectRosOpen();
    void tryOpenRoscore();
    bool splitMasterURI(const QString &uri, QString &master_hostname, QString &master_port);
    void testMasterReachable(const QString &master_hostname, int timeout = 2);
    void getHostInfo();
    void onRoscoreOpenFail(bool reset_toggle, const QString &err_msg);
    void onRoscoreOpenSuccess(const QString &info_msg);
    bool setROSBashEnv(QProcess* p);
    void showInfo(const QString &info);

    ErrorBadgeWidget *error_badge;

    QtMaterialRaisedButton *button_localhost;
    QtMaterialRaisedButton *button_localip;
    QtMaterialRaisedButton *button_dialog;

    QLabel *label_roscore_icon;
    QLabel *label_ros_path;

    QtMaterialCircularProgress *progress_open;

    QtMaterialTextField *text_rospath;

    QFileDialog *dialog_file;

    QtMaterialTextField *text_master_uri;
    QtMaterialSnackbar *snack;

    QFrame *mainframe;

    QProcess *process_roscore = nullptr;
    QProcess *process_ros_detect = nullptr;

    QTimer timer;
    QTimer timer_rosopen;
    QTimer timer_ros_detect;

    int ros_distro;
    QString ros_bash_path;
    QString host_ip;
    QString host_name;
    QString master_uri;

    bool is_rospath_changed = true;
    bool is_rospath_valid = false;
    bool is_master_local = false;
    bool open_roscore_if_master_reachable = false;

    const std::string ROS_INSTALL_PATH ="/opt/ros";
    const std::string ROS_DISTROS[ROS_DISTROS_NUM] = {
        "noetic",
        "melodic",
        "lunar",
        "kinetic",
        "jade",
        "indigo",
        "hydro"
        "groovy"
    };

    enum STATE {
        ROSCORE_CRASH,
        INITIALIZATION,
        WAIT_TO_START_ROSCORE,
        ROSCORE_OPENED
    } state;

    std::unordered_set<std::string> program_set;
    std::mutex register_mutex;

signals:
    void localRoscoreClosed();

private slots:
    void onStarted();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onPingFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void handleStandardOutput();
    void handleStandardError();
    void handlePingOutput();
    void handlePingError();
    void handleROSOpenOutput();
    void handleROSOpenError();
    void startRoscore();
    void haha(QProcess::ProcessError error);
    void buttonLocalhostClicked();
    void buttonLocalipClicked();
    void buttonDialogClicked();
    void checkRoscoreOpenResult();
    void onRospathChanged();
    void onToggled(bool tog);

    void openRoscore();

};

#endif // ROSCOREWIDGET_H
