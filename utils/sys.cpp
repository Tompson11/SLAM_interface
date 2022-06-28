#ifndef SYS_H
#define SYS_H

#include "unistd.h"
#include "shellpool.h"
#include <string>
#include <signal.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

namespace utils {
bool existDir(const std::string &dir) {
    return access(dir.c_str(), F_OK) == 0;
};

bool existDir(const QString &dir) {
    return existDir(dir.toStdString());
};

bool makeDir(const std::string &dir) {
    return mkdir(dir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) >= 0;
}

bool makeDir(const QString &dir) {
    return makeDir(dir.toStdString());
}

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

QStringList getSystemProcessID(QProcess *p_bash, const QString &p_name, const QString &ppid) {
    if(p_bash == nullptr) {
        auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
        p_bash = pool.getOneProcess();
    }

    QString cmd;
    QStringList arguments;
    if(ppid.isEmpty())
        cmd = "ps -aef | grep " + p_name + " | grep -v grep | grep -v bash | awk '{print $2}'";
    else
        cmd = "ps -aef | grep " + p_name + " | grep -v grep | grep -v bash | grep "  + ppid + " | awk '{print $2}'";

    arguments << "-c" << cmd;
    p_bash->setArguments(arguments);
    p_bash->start();
    p_bash->waitForFinished();

    QString out_str;
    utils::getQProcessStandardOutput(p_bash, out_str, true);

    return out_str.split("\n");
}

QStringList getAllChildProcessID(QProcess *p_bash, const QString &ppid) {
    if(p_bash == nullptr) {
        auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
        p_bash = pool.getOneProcess();
    }

    QString cmd = "ps -aef | awk '$3==" + ppid + " {print $2}'";
    QStringList arguments;
    arguments << "-c" << cmd;
    p_bash->setArguments(arguments);
    p_bash->start();
    p_bash->waitForFinished();

    QString out_str;
    utils::getQProcessStandardOutput(p_bash, out_str, true);

    return out_str.split("\n");
};

void killSystemProcess(QProcess *p_bash, const QString &p_name, const QString &ppid, int signal) {
    if(p_bash == nullptr) {
        auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
        p_bash = pool.getOneProcess();
    }

    QStringList pids = getSystemProcessID(p_bash, p_name, ppid);
    for(auto &pid : pids) {
        std::cout << "kill " << pid.toStdString() << std::endl;
        QString cmd = "kill -" + QString::number(signal) + " " + pid;
        QStringList arguments;
        arguments << "-c" << cmd;
        p_bash->setArguments(arguments);
        p_bash->start();
        p_bash->waitForFinished();
    }
}

void killAllChildProcess(QProcess *p_bash, const QString &ppid, int signal = SIGINT) {
    if(p_bash == nullptr) {
        auto &pool = utils::ShellPool<utils::SHELL_BASH>::getInstance();
        p_bash = pool.getOneProcess();
    }

    QStringList pids = getAllChildProcessID(p_bash, ppid);
    for(auto &pid : pids) {
        std::cout << "kill " << pid.toStdString() << std::endl;
        QString cmd = "kill -" + QString::number(signal) + " " + pid;
        QStringList arguments;
        arguments << "-c" << cmd;
        p_bash->setArguments(arguments);
        p_bash->start();
        p_bash->waitForFinished();
    }
}

};
#endif // SYS_H
