#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QPainter>
#include <QPaintEvent>
#include <QSequentialAnimationGroup>
#include "components/qtmaterialtoggle.h"
#include "components/qtmaterialraisedbutton.h"

class TitleWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color WRITE setColor READ color)
public:
    explicit TitleWidget(QWidget *parent = nullptr, const QColor& unact_color = QColor(255,255,255), const QColor& act_color = QColor(255,255,255));
    void playWinkAnimation();
    void playDisappearAnimation();
    void playAppearAnimation();
    void prepareDisappearAnimation(int frame_num);
    void prepareAppearAnimation(int frame_num);
    void setRemoveable(bool on);
    void notifyCreate();
    int getCompactHeight();
    virtual void toggleCompactLayout();

protected:
    void setColor(const QColor &color);
    QColor color() const;

    QColor current_color;
    QColor unactivated_color;
    QColor activated_color;

    QLineEdit *label_title;
    QLabel *label_title_back;

    QtMaterialToggle *toggle_start;
    QLabel *label_toggle_back;

    QtMaterialRaisedButton *button_remove = nullptr;
    bool removeable = false;

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

    QTimer timer_disappear;
    bool disappear_animation_ready = false;
    int disappear_step;

    QTimer timer_appear;
    bool appear_animation_ready = false;
    int appear_step;
    int appear_target_width;

    bool use_compact_layout = false;

protected slots:
    virtual void onButtonRemoveClicked();

signals:
    void titleWidgetCreate(QWidget*);
    void titleWidgetRemove(QWidget*);
    void titleWidgetChangePosition(QWidget*, QWidget*);
};

#endif // TITLEWIDGET_H
