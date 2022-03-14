#ifndef SYS_H
#define SYS_H

#include <QObject>
#include <QProcess>
#include <string>
#include <signal.h>
#include <iostream>
#include "unistd.h"

namespace utils {

bool existDir(const std::string &dir);
bool existDir(const QString &dir);
void getQProcessStandardOutput (QProcess *p, QString &out_str, bool trim = false);
void getQProcessStandardError (QProcess *p, QString &out_str, bool trim = false);
QStringList getSystemProcessID(QProcess *p_bash, const QString &p_name, const QString &ppid = QString());
QStringList getAllChildProcessID(QProcess *p_bash, const QString &ppid);
void killSystemProcess(QProcess *p_bash, const QString &p_name, const QString &ppid = QString(), int signal = SIGINT);
};

#endif // SYS_H
