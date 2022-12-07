#include "cmddelegate.h"


CmdDelegate::CmdDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
    timer_ =new QTimer(this);
    timer_->setSingleShot(true);
    connect(timer_, SIGNAL(timeout()), this, SLOT(execCmd()));
}

QWidget* CmdDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    editor->setFrame(QFrame::NoFrame);
    return editor;
}

void CmdDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QLineEdit *e = static_cast<QLineEdit*>(editor);
    model->setData(index, e->text());
}

void CmdDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    viewOption.decorationAlignment = Qt::AlignCenter;
    viewOption.decorationPosition = QStyleOptionViewItem::Top;
    viewOption.decorationSize = QSize(20,20);
    QStyledItemDelegate::paint(painter, viewOption, index);
}

bool CmdDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);

    if(mouseEvent) {
        auto event_type = mouseEvent->type();
        auto event_button = mouseEvent->button();

        if(event_type == QEvent::MouseButtonPress) {
            if(event_button == Qt::LeftButton) {
                timer_->start(500);
            }
            else if(event_button == Qt::RightButton) {
                QDialog().exec();
            }
        }
        else if(event_type == QEvent::MouseButtonDblClick) {
            if(event_button == Qt::LeftButton) {
                timer_->stop();
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void CmdDelegate::execCmd() {
    std::cout << "exec!" << std::endl;
}
