#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "components/qtmaterialtoggle.h"

class TitleWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color WRITE setColor READ color)
public:
    explicit TitleWidget(QWidget *parent = nullptr, const QColor& unact_color = QColor(255,255,255), const QColor& act_color = QColor(255,255,255));
    void playWinkAnimation();

protected:
    void setColor(const QColor &color);
    QColor color() const;

    QColor current_color;
    QColor unactivated_color;
    QColor activated_color;

    QLabel *label_title;
    QLabel *label_title_back;

    QtMaterialToggle *toggle_start;
    QLabel *label_toggle_back;

    // layout
    QGridLayout *title_layout;
    QGridLayout *body_layout;
    QVBoxLayout *main_layout;

    // frame
    QFrame *body_frame;
    QFrame *title_frame;

    //* animation
    QPropertyAnimation *animation_activate;
    QPropertyAnimation *animation_unactivate;
    QSequentialAnimationGroup *animation_wink_forward;

    QPropertyAnimation *animation_activate_backward;
    QPropertyAnimation *animation_unactivate_backward;
    QSequentialAnimationGroup *animation_wink_backward;

signals:

};

#endif // TITLEWIDGET_H
