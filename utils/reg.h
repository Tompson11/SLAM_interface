#ifndef REG_H
#define REG_H

#include <QObject>
#include <QRegExp>

namespace utils {
    QStringList extractNumberAfterString(const QString &src_str, const QString &tag_str);
}

#endif // REG_H
