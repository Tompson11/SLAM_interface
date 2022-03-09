#include "reg.h"
#include <iostream>

namespace utils {

QStringList extractNumberAfterString(const QString &src_str, const QString &tag_str) {
    QRegExp re("(?:" + tag_str + ")(\\d+\\.?\\d*)");
    QStringList ret;

    int pos = 0;
    while ((pos = re.indexIn(src_str, pos)) != -1) {
        ret << re.cap(1);
        pos += re.matchedLength();
    }
    return ret;
};

}
