#include "reg.h"
#include <iostream>

namespace utils {

QStringList extractAllByReg(const QRegExp &re, const QString &str) {
    QStringList ret;

    int pos = 0;
    while ((pos = re.indexIn(str, pos)) != -1) {
        ret << re.cap(1);
        pos += re.matchedLength();
    }
    return ret;
}

QStringList extractNumberAfterString(const QString &src_str, const QString &tag_str) {
    QRegExp re("(?:" + tag_str + ")(\\d+\\.?\\d*)");
    return extractAllByReg(re, src_str);
};

QStringList extractStringAfterString(const QString &src_str, const QString &tag_str) {
    QRegExp re("(?:" + tag_str + ")(*)");
    return extractAllByReg(re, src_str);
}

QStringList extractStringBetweenString(const QString &src_str, const QString &tag_str_front, const QString &tag_str_back) {
    QRegExp re("(?:" + tag_str_front + ")([\\s\\S]*)(?:" + tag_str_back + ")");
    return extractAllByReg(re, src_str);
}

}
