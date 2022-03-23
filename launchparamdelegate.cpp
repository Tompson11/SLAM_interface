#include "launchparamdelegate.h"
#include <iostream>

LaunchParamDelegate::LaunchParamDelegate(QWidget *parent, QAbstractItemModel *model) : QStyledItemDelegate(parent)
{
    model_ = static_cast<QStandardItemModel*>(model);
}

QWidget* LaunchParamDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.column() == PARAMETER_VALUE) {
        QLineEdit *line_edit = new QLineEdit(parent);
        QString type = model_->data(index.siblingAtColumn(PARAMETER_TYPE)).toString();

        if(type == "int") {
            line_edit->setValidator(new QIntValidator(parent));
        }
        else if(type == "double") {
            line_edit->setValidator(new QDoubleValidator(parent));
        }
        else if(type == "bool") {
            QRegExp bool_re("[01]");
            line_edit->setValidator(new QRegExpValidator(bool_re, parent));
        }
        else if(type == "string") {

        }

        return line_edit;
    }

}

void LaunchParamDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if(index.column() == PARAMETER_VALUE) {
        QLineEdit *line_edit = static_cast<QLineEdit *>(editor);
        model->setData(index, line_edit->text());
    }
}

void LaunchParamDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    QStyledItemDelegate::paint(painter, viewOption, index);

    if (index.column() == PARAMETER_RESET)
    {
        QStyleOptionButton check_box_style_option;
        bool is_pressed = model_->data(index, Qt::UserRole).toBool();
        if(is_pressed == false){
            check_box_style_option.rect = option.rect;
        }
        else {
            check_box_style_option.rect = QRect(option.rect.left() + 4, option.rect.top() + 2, option.rect.width() - 8, option.rect.height() - 4);
        }

        auto *parent_item = model_->itemFromIndex(index.parent());
        auto *header_item = model_->itemFromIndex(index.siblingAtColumn(PARAMETER_NAMESPACE));

        if(header_item == model_->item(0) || header_item == model_->item(1)) {
            check_box_style_option.palette.setColor(QPalette::Button, is_pressed ? QColor( 241, 148, 138 ) : QColor( 203, 67, 53 ));
        }
        else if(parent_item == model_->item(1)) {
            check_box_style_option.palette.setColor(QPalette::Button, is_pressed ? QColor(  133, 193, 233 ) : QColor( 46, 134, 193 ));
        }
        else {
            check_box_style_option.palette.setColor(QPalette::Button, is_pressed ? QColor( 249, 231, 159 ) : QColor( 241, 196, 15 ));
        }

        check_box_style_option.text = "reset";

        QApplication::style()->drawControl(QStyle::CE_PushButton,&check_box_style_option, painter);
    }

}

bool LaunchParamDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (index.column() == PARAMETER_RESET) {

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if(event->type() == QEvent::MouseButtonPress) {
                model->setData(index, true, Qt::UserRole);

                auto *parent_item = model_->itemFromIndex(index.parent());
                auto *header_item = model_->itemFromIndex(index.siblingAtColumn(PARAMETER_NAMESPACE));
                if(parent_item == model_->item(1) || header_item == model_->item(0)) {
                    resetParamsOfCertainNamespace(header_item);
                }
                else if(header_item == model_->item(1)){
                    resetParamsOAllNamespace(header_item);
                }
                else
                {
                    model->setData(index.siblingAtColumn(PARAMETER_VALUE), model->data(index.siblingAtColumn(PARAMETER_VALUE), Qt::UserRole), Qt::DisplayRole);
                }
            }
            else if(event->type() == QEvent::MouseButtonRelease) {
                model->setData(index, false, Qt::UserRole);
            }
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize LaunchParamDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight( size.height() + 4 );
    return size;
}

void LaunchParamDelegate::resetParamsOAllNamespace(QStandardItem *ns_header_item) {
    for(int row = 0; row < ns_header_item->rowCount(); row++){
        auto *ns_item = ns_header_item->child(row);
        resetParamsOfCertainNamespace(ns_item);
    }
}

void LaunchParamDelegate::resetParamsOfCertainNamespace(QStandardItem *ns_header_item) {
    for(int row = 0; row < ns_header_item->rowCount(); row++){
        auto *value_item = ns_header_item->child(row, 3);
        value_item->setData(value_item->data(Qt::UserRole), Qt::DisplayRole);
    }
}
