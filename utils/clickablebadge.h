#ifndef CLICKABLEBADGE_H
#define CLICKABLEBADGE_H

#include "components/qtmaterialbadge.h"

class ClickableBadge : public QtMaterialBadge
{
    Q_OBJECT
public:
    ClickableBadge(QWidget *parent = nullptr);

};

#endif // CLICKABLEBADGE_H
