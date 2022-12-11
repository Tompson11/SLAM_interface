#include "clickablebadge.h"

clickableBadge::clickableBadge(QWidget *parent) : QtMaterialBadge(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void clickableBadge::enterEvent(QEvent *e) {

}

void clickableBadge::mousePressEvent(QMouseEvent *e) {
    emit clicked();
}
