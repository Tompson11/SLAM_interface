#include "focusincombobox.h"
#include <iostream>
FocusInComboBox::FocusInComboBox(QWidget *parent) : QComboBox(parent)
{

}

void FocusInComboBox::focusInEvent(QFocusEvent *e) {
    this->clearFocus();
    emit focusIn();
}

void FocusInComboBox::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::RightButton)
        emit rightButtonPressed();
    else
        QComboBox::mousePressEvent(e);
}
