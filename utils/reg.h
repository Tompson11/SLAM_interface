#ifndef REG_H
#define REG_H

#include <QObject>
#include <QRegExp>

namespace utils {
    QStringList extractAllByReg(const QRegExp &re, const QString &str);
    QStringList extractNumberAfterString(const QString &src_str, const QString &tag_str);
    QStringList extractStringAfterString(const QString &src_str, const QString &tag_str);
    QStringList extractStringBetweenString(const QString &src_str, const QString &tag_str_front, const QString &tag_str_back);
}

#endif // REG_H
