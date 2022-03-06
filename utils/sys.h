#ifndef SYS_H
#define SYS_H

#include "unistd.h"
#include "shellpool.h"
#include <string>
#include <signal.h>
#include <iostream>

namespace utils {
bool existDir(const std::string &dir) {
    return access(dir.c_str(), F_OK) == 0;
};

void getQProcessStandardOutput (QProcess *p, QString &out_str, bool trim = false) {
    if(p) {
        out_str = QString::fromLocal8Bit(p->readAllStandardOutput());
        if(trim)
            out_str = out_str.trimmed();
    }
}

void getQProcessStandardError (QProcess *p, QString &out_str, bool trim = false) {
    if(p) {
        out_str = QString::fromLocal8Bit(p->readAllStandardError());
        if(trim)
            out_str = out_str.trimmed();
    }
}

QString getSystemProcessID(QProcess *p_bash, const QString &p_name) {
    if(p_bash == nullptr) {
        auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
        p_bash = pool.getOneProcess();
    }

    QString cmd;
    QStringList arguments;
    cmd = "ps -aux | grep " + p_name + " | grep -v grep | grep -v bash | awk '{print $2}'";
    arguments << "-c" << cmd;
    p_bash->setArguments(arguments);
    p_bash->start();
    p_bash->waitForFinished();

    QString out_str = QString::fromLocal8Bit(p_bash->readAllStandardOutput());
    std::cout << out_str.toStdString() << std::endl;

    QString out_str_trimmed = out_str.trimmed();

    return out_str_trimmed;
}

void killSystemProcess(QProcess *p_bash, const QString &p_name, int signal = SIGINT) {
    if(p_bash == nullptr) {
        auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
        p_bash = pool.getOneProcess();
    }

    QString pid = getSystemProcessID(p_bash, p_name);
    if(pid.length()) {
        QString cmd = "kill -" + QString::number(signal) + " " + pid;
        QStringList arguments;
        arguments << "-c" << cmd;
        p_bash->setArguments(arguments);
        p_bash->start();
        p_bash->waitForFinished();
    }
}

}
#endif // SYS_H
