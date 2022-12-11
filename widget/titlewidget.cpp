#include "titlewidget.h"

TitleWidget::TitleWidget(QWidget *parent, const QColor& unact_color, const QColor& act_color) :
    QWidget(parent), unactivated_color(unact_color), activated_color(act_color)
{
    label_title = new QLineEdit(this);
    label_title->setAlignment(Qt::AlignCenter);
    label_title->setStyleSheet("background:transparent;border-width:0;border-style:outset;"
                               "font-size: 20px;"
                               "font-weight: bold;"
                               );
//    QFont default_font = label_title->font();
//    default_font.setBold(true);
//    default_font.set
//    label_title->setFont()

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
    title_frame->setFixedHeight(48);
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
    main_layout->addWidget(body_frame, 0, Qt::AlignTop);
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

    timer_disappear.setInterval(10);
    connect(&timer_disappear, &QTimer::timeout, this, &TitleWidget::playDisappearAnimation);

    timer_appear.setInterval(10);
    connect(&timer_appear, &QTimer::timeout, this, &TitleWidget::playAppearAnimation);

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

int TitleWidget::getCompactHeight() {
    return 66;
}

void TitleWidget::toggleCompactLayout() {
    return;
}

void TitleWidget::playWinkAnimation() {
    if(toggle_start->isChecked()) {
        animation_wink_backward->start();
    }
    else {
        animation_wink_forward->start();
    }
}

void TitleWidget::prepareDisappearAnimation(int frame_num) {
    disappear_step = this->width() / frame_num;
    disappear_animation_ready = true;
    timer_disappear.start();
}

void TitleWidget::prepareAppearAnimation(int frame_num) {
    appear_target_width = this->width();
    this->setFixedSize(0, this->height());
    appear_step = appear_target_width / frame_num;
    appear_animation_ready = true;
    timer_appear.start();
}

void TitleWidget::playDisappearAnimation() {
    if(disappear_animation_ready == false)
        return;

    int cur_width = this->width();
    int new_width = cur_width - disappear_step;
    if(new_width < 0)
        new_width = 0;

    this->setFixedWidth(new_width);
    if(new_width == 0) {
        disappear_animation_ready = false;
        timer_disappear.stop();
        emit titleWidgetRemove(this);
    }
}

void TitleWidget::playAppearAnimation() {
    if(appear_animation_ready == false)
        return;

    int cur_width = this->width();
    int new_width = cur_width + appear_step;
    if(new_width > appear_target_width)
        new_width = appear_target_width;

    this->setFixedWidth(new_width);
    if(new_width == appear_target_width) {
        appear_animation_ready = false;
        timer_appear.stop();
    }
}

void TitleWidget::setRemoveable(bool on) {
    removeable = on;
    if(removeable) {
        if(button_remove == nullptr) {
            button_remove = new QtMaterialRaisedButton(this);
            button_remove->setText("x");
            title_layout->addWidget(button_remove, 0, 5, 1, 1, Qt::AlignRight);
            connect(button_remove, &QtMaterialRaisedButton::clicked, this, &TitleWidget::onButtonRemoveClicked);
        }
        else {
            button_remove->show();
        }
    }
    else if(button_remove){
        button_remove->hide();
    }
}

void TitleWidget::onButtonRemoveClicked() {
    emit titleWidgetRemove(this);
}

void TitleWidget::notifyCreate() {
    emit titleWidgetCreate(this);
}
