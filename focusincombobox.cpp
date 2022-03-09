#include "focusincombobox.h"
#include <iostream>
FocusInComboBox::FocusInComboBox(QWidget *parent) : QComboBox(parent)
{

}

void FocusInComboBox::focusInEvent(QFocusEvent *e) {
    this->clearFocus();
    emit focusIn();
}
