#include "titlewidget.h"

TitleWidget::TitleWidget(QWidget *parent, const QColor& unact_color, const QColor& act_color) :
    QWidget(parent), unactivated_color(unact_color), activated_color(act_color)
{
    label_title = new QLabel(this);
    label_title_back = new QLabel(this);
    label_title_back->setMinimumHeight(48);

    toggle_start = new QtMaterialToggle(this);

    label_toggle_back = new QLabel(this);

    title_layout = new QGridLayout();
    title_layout->addWidget(label_toggle_back, 0, 0, 1, 1);
    title_layout->addWidget(toggle_start, 0, 0, 1, 1, Qt::AlignCenter);
    title_layout->addWidget(label_title_back, 0, 1, 1, 1);
    title_layout->addWidget(label_title, 0, 1, 1, 1, Qt::AlignCenter);
    title_layout->setSpacing(0);
    title_layout->setMargin(0);

    title_frame = new QFrame(this);
    title_frame->setLayout(title_layout);
    title_frame->setStyleSheet(".QFrame{"
                               "border-top-left-radius: 15px;"
                               "border-top-right-radius: 15px;"
                               "}"
                               );

    body_layout = new QGridLayout();

    body_frame = new QFrame(this);
    body_frame->setLayout(body_layout);

    main_layout = new QVBoxLayout();
    main_layout->addWidget(title_frame, 0);
    main_layout->addWidget(body_frame, 0);
    main_layout->setSpacing(0);
    this->setLayout(main_layout);

    animation_activate = new QPropertyAnimation(this);
    animation_activate->setPropertyName("color");
    animation_activate->setTargetObject(this);
    animation_activate->setStartValue(unactivated_color);
    animation_activate->setEndValue(activated_color);
    animation_activate->setDuration(300);

    animation_unactivate = new QPropertyAnimation(this);
    animation_unactivate->setPropertyName("color");
    animation_unactivate->setTargetObject(this);
    animation_unactivate->setStartValue(activated_color);
    animation_unactivate->setEndValue(unactivated_color);
    animation_unactivate->setDuration(300);

    animation_wink_forward = new QSequentialAnimationGroup(this);
    animation_wink_forward->setLoopCount(2);
    animation_wink_forward->addAnimation(animation_activate);
    animation_wink_forward->addAnimation(animation_unactivate);

    animation_activate_backward = new QPropertyAnimation(this);
    animation_activate_backward->setPropertyName("color");
    animation_activate_backward->setTargetObject(this);
    animation_activate_backward->setStartValue(unactivated_color);
    animation_activate_backward->setEndValue(activated_color);
    animation_activate_backward->setDuration(300);

    animation_unactivate_backward = new QPropertyAnimation(this);
    animation_unactivate_backward->setPropertyName("color");
    animation_unactivate_backward->setTargetObject(this);
    animation_unactivate_backward->setStartValue(activated_color);
    animation_unactivate_backward->setEndValue(unactivated_color);
    animation_unactivate_backward->setDuration(300);

    animation_wink_backward = new QSequentialAnimationGroup(this);
    animation_wink_backward->setLoopCount(2);
    animation_wink_backward->addAnimation(animation_unactivate_backward);
    animation_wink_backward->addAnimation(animation_activate_backward);

    this->setColor(unact_color);
}

void TitleWidget::setColor(const QColor &color) {
    this->current_color = color;

    QString rgb_str = QString("rgb(%1,%2,%3)").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));
    QString qss_title = QString(".QFrame{border:2px solid %1;background-color: %2;}").arg(rgb_str, rgb_str);
    title_frame->setStyleSheet(qss_title);

    QString qss_body = QString(".QFrame{border:2px solid %1;}").arg(rgb_str);
    body_frame->setStyleSheet(qss_body);

    update();
}

QColor TitleWidget::color() const {
    return this->current_color;
}

void TitleWidget::playWinkAnimation() {
    if(toggle_start->isChecked()) {
        animation_wink_backward->start();
    }
    else {
        animation_wink_forward->start();
    }
}
