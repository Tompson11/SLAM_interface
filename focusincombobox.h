#ifndef FOCUSINCOMBOBOX_H
#define FOCUSINCOMBOBOX_H

#include <QComboBox>
#include <QObject>
#include <QWidget>
#include <QFocusEvent>

class FocusInComboBox : public QComboBox
{
    Q_OBJECT
public:
    FocusInComboBox(QWidget *parent = nullptr);

signals:
    void focusIn();
    void rightButtonPressed();

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
};

#endif // FOCUSINCOMBOBOX_H
