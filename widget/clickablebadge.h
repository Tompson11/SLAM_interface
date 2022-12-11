#ifndef CLICKABLEBADGE_H
#define CLICKABLEBADGE_H

#include <QObject>
#include <QWidget>
#include "components/qtmaterialbadge.h"
#include <QEnterEvent>
#include <iostream>
#include <QMouseEvent>
class clickableBadge : public QtMaterialBadge
{
    Q_OBJECT
public:
    explicit clickableBadge(QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void clicked();

};

#endif // CLICKABLEBADGE_H
