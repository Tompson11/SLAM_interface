#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class SensorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SensorWidget(QWidget *parent = nullptr);
//    void enterEvent(QEvent *e);
//    void leaveEvent(QEvent *e);

private:
    QLabel *label_background;
    QColor background_color;
signals:

};

#endif // SENSORWIDGET_H
