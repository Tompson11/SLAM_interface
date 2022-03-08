#include "roscorewidget.h"
#include "utils/shellpool.h"
#include "utils/sys.h"
#include <iostream>

RoscoreWidget::RoscoreWidget(QWidget *parent) : QWidget(parent)
{
    this->state = INITIALIZATION;

    text_rospath = new QtMaterialTextField(this);
    text_rospath->setLabel("ROS Installment Path");

    button_dialog = new QtMaterialRaisedButton(this);
    button_dialog->setText("browse");

    dialog_file = new QFileDialog(this);
    dialog_file->setWindowTitle("Choose Source Directory");
    dialog_file->setFileMode( QFileDialog::DirectoryOnly );

    label_roscore_icon = new QLabel(this);
    label_roscore_icon->setText("Roscore Icon");

    toggle_start = new QtMaterialToggle(this);

    text_master_uri = new QtMaterialTextField(this);
    text_master_uri->setLabel("ROS_MASTER_URI");

    button_localhost = new QtMaterialRaisedButton(this);
    button_localhost->setText("Use Localhost");

    button_localip = new QtMaterialRaisedButton(this);
    button_localip->setText("Use Local IP");

    snack = new QtMaterialSnackbar(this);

    progress_open = new QtMaterialCircularProgress(this);
    progress_open->setVisible(false);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(label_roscore_icon, 0, 0, 2, 1);
    layout->addWidget(progress_open, 0, 0, 2, 1);
    layout->addWidget(toggle_start, 2, 0, 1, 1);
    layout->addWidget(text_rospath, 0, 1 ,1, 1);
    layout->addWidget(button_dialog, 0, 2, 1, 2);
    layout->addWidget(text_master_uri, 1, 1, 1, 1);
    layout->addWidget(button_localhost, 1, 2, 1, 1);
    layout->addWidget(button_localip, 1, 3, 1, 1);
    layout->addWidget(snack, 2, 1, 1, 3);
    this->setLayout(layout);

    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    process_roscore = pool.getOneProcess();

    connect(button_localhost, &QtMaterialRaisedButton::clicked, this, &RoscoreWidget::buttonLocalhostClicked);
    connect(button_localip, &QtMaterialRaisedButton::clicked, this, &RoscoreWidget::buttonLocalipClicked);
    connect(button_dialog, &QtMaterialRaisedButton::clicked, this, &RoscoreWidget::buttonDialogClicked);

    connect(toggle_start, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));

    connect(text_rospath, &QtMaterialTextField::textChanged, this, &RoscoreWidget::onRospathChanged);

    connect(process_roscore, &QProcess::started, this, &RoscoreWidget::onStarted);
    connect(process_roscore, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),this, &RoscoreWidget::onFinished);
    connect(process_roscore, &QProcess::readyReadStandardOutput,this,&RoscoreWidget::handleStandardOutput);
    connect(process_roscore, &QProcess::readyReadStandardError,this,&RoscoreWidget::handleStandardError);
    connect(process_roscore, SIGNAL(errorOccurred(QProcess::ProcessError)), this,SLOT(haha(QProcess::ProcessError)));

    connect(&timer_ros_detect, &QTimer::timeout, this, &RoscoreWidget::handleROSOpenError);
    connect(&timer_rosopen, &QTimer::timeout, this, &RoscoreWidget::detectRosOpen);

    text_rospath->setText("/opt/ros/noetic");
}

RoscoreWidget::~RoscoreWidget() {
    if(process_roscore->state() == QProcess::Running) {
        process_roscore->kill();
        process_roscore->waitForFinished();
    }
}

void RoscoreWidget::testMasterReachable(const QString &master_hostname, int timeout) {
//    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
//    process_bash = pool.getOneProcess();
//    connect(process_bash, &QProcess::readyReadStandardOutput,this,&RoscoreWidget::handlePingOutput);
//    connect(process_bash, &QProcess::readyReadStandardError,this,&RoscoreWidget::handlePingError);

//    std::string cmd;
//    cmd = "ping " + master_hostname.toStdString() + " -w " + std::to_string(timeout) + "| grep 'rtt'\n";
//    process_bash->write(cmd.c_str());


//    QString outStr = QString::fromLocal8Bit(process_bash->readAllStandardOutput());
//    pool.returnOneProcess(process_bash);
}

void RoscoreWidget::onRospathChanged() {
    is_rospath_changed = true;
    is_rospath_valid = false;
    ros_bash_path = text_rospath->text() + "/setup.bash";
}

void RoscoreWidget::onToggled(bool tog) {
     if(tog) {
        toggle_start->setEnabled(false);
        progress_open->setVisible(true);

        text_master_uri->setEnabled(false);
        text_rospath->setEnabled(false);
        button_dialog->setEnabled(false);
        button_localhost->setEnabled(false);
        button_localip->setEnabled(false);

        this->state = WAIT_TO_START_ROSCORE;
        tryOpenRoscore();
     }
     else{
         if(is_master_local) {
             toggle_start->setEnabled(false);
             progress_open->setVisible(true);

             utils::killSystemProcess(nullptr, "roscore", QString::number(process_roscore->pid()));
             process_roscore->terminate();
             process_roscore->waitForFinished();

             onRoscoreOpenFail(false, "");
         }
     }
}

bool RoscoreWidget::splitMasterURI(const QString &uri, QString &master_hostname, QString &master_port) {
    if(!uri.startsWith("http") || uri.length() < 8)
        return false;

    int p = 4;
    if(uri.at(p) == 's') {
        p++;
    }
    if(uri.mid(p, 3) != "://")
        return false;

    int ip_start_pos =  p + 3;
    int ip_num = 0;
    int ip_value = 0;
    auto it = uri.begin() + ip_start_pos;
    for(; it != uri.end(); it++) {
        if(!it->isNumber()) {
            if(*it == ':') {
                if(ip_num > 0)
                    ip_num++;

                break;
            }
            else if(*it == '.') {
                if(it == uri.begin() + ip_start_pos)
                    return false;

                ip_value = 0;
                ip_num++;
            }
            else if(!(it->isLetter() || *it == '-')) {
                if(ip_num > 0)
                    return false;
            }
        }
        else {
            ip_value = ip_value * 10 + (it->toLatin1() - '0');

            if(ip_value > 255) {
                return false;
            }
        }
    }

    if(ip_num == 0 || ip_num == 4) {
        master_hostname = uri.mid(ip_start_pos, it - uri.begin() - ip_start_pos);
    }
    else {
        return false;
    }

    if(it == uri.end() || *it != ':')
        return false;

    it++;
    auto it_port_start = it;
    int port_value = 0;
    for(; it != uri.end(); it++) {
        if(it->isNumber()) {
           port_value = port_value * 10 + (it->toLatin1() - '0');

           if(port_value > 65535)
               return false;
        }
        else if(*it != '/'){
            return false;
        }
        else if(uri.end() - it > 1){
            return false;
        }
    }

    master_port = uri.mid(it_port_start - uri.begin(), it - it_port_start);
    return true;

}

void RoscoreWidget::onRoscoreOpenSuccess(const QString &info_msg) {
    this->state = ROSCORE_OPENED;

    toggle_start->setEnabled(true);
    progress_open->setVisible(false);

    showInfo(info_msg);
}

void RoscoreWidget::onRoscoreOpenFail(bool reset_toggle, const QString &err_msg) {
    this->state = INITIALIZATION;

    toggle_start->setEnabled(true);
    progress_open->setVisible(false);

    text_master_uri->setEnabled(true);
    text_rospath->setEnabled(true);
    button_dialog->setEnabled(true);
    button_localhost->setEnabled(true);
    button_localip->setEnabled(true);

    if(reset_toggle) {
        toggle_start->disconnect(SIGNAL(toggled(bool)),this, SLOT(onToggled(bool)));
        toggle_start->toggle();
        connect(toggle_start, SIGNAL(toggled(bool)),this, SLOT(onToggled(bool)));
    }

    if(err_msg.length())
        showInfo(err_msg);
}

void RoscoreWidget::tryOpenRoscore() {
    if(this->state == WAIT_TO_START_ROSCORE) {
        QString master_hostname;
        QString master_port;
        if(splitMasterURI(text_master_uri->text().trimmed(), master_hostname, master_port)) {
            this->master_uri = text_master_uri->text().trimmed();
            getHostInfo();
            if(master_hostname == this->host_name || master_hostname == "localhost" ||
               master_hostname == this->host_ip || master_hostname == "127.0.0.1")
            {
                this->is_master_local = true;
                openRoscore();
            }
            else {
                this->is_master_local = false;
                detectRosOpen();
            }
        }
        else {
            onRoscoreOpenFail(true, "Please Input Valid ROS_MASTER_URI");
        }
    }

}

bool RoscoreWidget::getSourceROSCmd(QString &cmd) {

    if(is_rospath_changed) {
        is_rospath_valid = validateRosPath(ros_bash_path, true);
    }

    if(!is_rospath_valid)
        return false;

    cmd.clear();
    cmd += "source " + ros_bash_path + ";";
    cmd += "export ROS_MASTER_URI=" + this->master_uri + ";";
    cmd += "export ROS_HOSTNAME=" + this->host_ip + ";";

    return true;

}

void RoscoreWidget::detectRosOpen() {
       auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
       process_ros_detect = pool.getOneProcess();
       connect(process_ros_detect, &QProcess::readyReadStandardOutput,this,&RoscoreWidget::handleROSOpenOutput);
       connect(process_ros_detect, &QProcess::readyReadStandardError,this,&RoscoreWidget::handleROSOpenError);

       QString cmd;
       if(!getSourceROSCmd(cmd)) {
            if(this->state == WAIT_TO_START_ROSCORE)
                onRoscoreOpenFail(true, "ROS Install Path Unvalid!");
           return;
       }

       QStringList arguments;
       cmd += "rostopic list";
       arguments << "-c" << cmd;
       process_ros_detect->setArguments(arguments);
       process_ros_detect->start();

       timer_ros_detect.setInterval(3000);
       timer_ros_detect.start();

}

void RoscoreWidget::showInfo(const QString &info) {
    snack->setToolTipDuration(1000);
    snack->addInstantMessage(info);
}

bool RoscoreWidget::validateRosPath(const QString &bash_path, bool pop_error) {
    if(bash_path.length() > 0) {
        if(utils::existDir(bash_path)) {
            return true;
        }
        else {
            if(pop_error)
                showInfo("PATH Unvalid!");

            return false;
        }
    }
    else {
        return false;
    }
}

void RoscoreWidget::onStarted() {
//    ros_distro = sourceRosBash();

//    if(ros_distro < 0) {
//        this->state = ROS_BAD;
//    }
//    else {
//        this->state = WAIT_TO_START_ROSCORE;
////        tryOpenRoscore();
//    }
}

void RoscoreWidget::onPingFinished(int exitCode, QProcess::ExitStatus exitStatus) {
//    QString out;
//    utils::getQProcessStandardError(process_bash, out);
//    utils::getQProcessStandardOutput(, out);
}

void RoscoreWidget::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    std::cout << exitCode << " " << exitStatus << std::endl;
}

void RoscoreWidget::haha(QProcess::ProcessError error) {
    std::cout << error << std::endl;
}

void RoscoreWidget::handlePingOutput() {
    if(open_roscore_if_master_reachable) {
        open_roscore_if_master_reachable = false;
        openRoscore();
    }
};

void RoscoreWidget::handlePingError() {
    open_roscore_if_master_reachable = false;
};

void RoscoreWidget::handleStandardOutput() {
    QString out;
    utils::getQProcessStandardOutput(process_roscore, out, true);

    std::cout << out.toStdString() << std::endl;
}

void RoscoreWidget::handleStandardError() {
    QString out_str;
    utils::getQProcessStandardError(process_roscore, out_str, true);

    std::cout << out_str.toStdString() << std::endl;

    if(out_str.length()) {
        if(this->state == WAIT_TO_START_ROSCORE) {
            timer_ros_detect.stop();
            timer_rosopen.stop();
            if(out_str.indexOf("roscore cannot run as another roscore/master is already running") >= 0) {
                setMasterURI(this->master_uri);
                onRoscoreOpenSuccess("ROSCORE Opened!");
            }
            else if(out_str.indexOf("host is not set to this machine") >= 0) {
                setMasterURI(this->master_uri);
                onRoscoreOpenSuccess("ROSCORE Opened!");
            }
        }
    }
}

void RoscoreWidget::handleROSOpenOutput() {
    timer_ros_detect.stop();
    timer_rosopen.stop();

    if(this->state == WAIT_TO_START_ROSCORE)
        onRoscoreOpenSuccess("ROSCORE Opened!");

    utils::ShellPool<utils::SHELL_BASH>::getInstance().returnOneProcess(process_ros_detect);
}

void RoscoreWidget::handleROSOpenError(){
    timer_ros_detect.stop();
    timer_rosopen.stop();

    QString out;
    utils::getQProcessStandardError(process_ros_detect ,out, true);

    if(out.length() == 0) {
        utils::killSystemProcess(nullptr, "rostopic");
        process_ros_detect->terminate();
        process_ros_detect->waitForFinished();
        utils::ShellPool<utils::SHELL_BASH>::getInstance().returnOneProcess(process_ros_detect);
        out = "no!";
    }

    onRoscoreOpenFail(true, out);
}

void RoscoreWidget::checkRoscoreOpenResult() {
    QString out;
    utils::getQProcessStandardOutput(process_roscore ,out, true);

    if(out.isEmpty()) {
        utils::killSystemProcess(nullptr, "roscore", QString::number(process_roscore->pid()));
    }

    QString err;
    utils::getQProcessStandardError(process_roscore ,err);
}

void RoscoreWidget::openRoscore() {
    if(this->state == WAIT_TO_START_ROSCORE) {
        QString cmd;
        if(!getSourceROSCmd(cmd)) {
            onRoscoreOpenFail(true, "ROS Install Path Unvalid!");
            return;
        }

        QStringList arguments;
        cmd += "roscore";
        arguments << "-c" << cmd;
        process_roscore->setArguments(arguments);
        process_roscore->start();

        timer_rosopen.setInterval(3000);
        timer_rosopen.start();
    }
}

void RoscoreWidget::startRoscore() {
    process_roscore->start("export ROS_='https://183.173.114.252:11311';roscore");
}

void RoscoreWidget::getHostInfo() {
    auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
    QProcess *p = pool.getOneProcess();

    QStringList arguments;
    arguments << "-c" << "hostname -I | awk '{print $1}'";
    p->setArguments(arguments);
    p->start();
    p->waitForFinished();

    QString out_str;
    utils::getQProcessStandardOutput(p, out_str, true);
    if(out_str.length() > 0) {
        this->host_ip = out_str;
    }
    else{
        this->host_ip = "127.0.0.1";
    }

    arguments.clear();
    arguments << "-c" << "hostname -s | awk '{print $1}'";
    p->setArguments(arguments);
    p->start();
    p->waitForFinished();

    utils::getQProcessStandardOutput(p, out_str, true);
    if(out_str.length() > 0) {
        this->host_name = out_str;
    }
    else{
        this->host_name = "localhost";
    }

    pool.returnOneProcess(p);
}

void RoscoreWidget::buttonDialogClicked() {
    QStringList fileName;
    if ( dialog_file->exec() == QDialog::Accepted )
    {
        fileName = dialog_file->selectedFiles();
        text_rospath->setText(fileName[0]);
    }
    else
    {
        return;
    }
}

void RoscoreWidget::buttonLocalhostClicked() {
    setMasterURI("http://localhost:11311");
}

void RoscoreWidget::buttonLocalipClicked() {
    getHostInfo();
    setMasterURI(QString::fromStdString("http://" + this->host_ip.toStdString() + ":11311"));
}

void RoscoreWidget::setMasterURI(const QString &uri) {
    text_master_uri->setText(uri);
}
