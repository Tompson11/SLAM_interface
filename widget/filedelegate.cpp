#include "launchtableview.h"
#include "filedelegate.h"
#include "utils/sys.h"
#include <iostream>

FileDelegate::FileDelegate(QWidget *parent, LaunchTableView *table_view): QStyledItemDelegate(parent), tableview(table_view)
{

}


QWidget* FileDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.column() == 0) {
        QLineEdit *line_edit_name = new QLineEdit(parent);
        return line_edit_name;
    }
    else if(index.column() == 1){
        QFileDialog *dialog_dir = new QFileDialog(parent);
        dialog_dir->setMinimumSize(640, 480);
        dialog_dir->setWindowTitle("Choose Workspace Directory");
        dialog_dir->setFileMode( QFileDialog::DirectoryOnly );

        auto *model = tableview->model();
        QString ori_dir = model->data(index).toString();
        if(!ori_dir.isEmpty())
            dialog_dir->setDirectory(ori_dir);

        return dialog_dir;
    }
    else if(index.column() == 2){
        QFileDialog *dialog_file = new QFileDialog(parent);
        auto *model = tableview->model();
        QString target_dir = model->data(model->index(index.row(), index.column() - 1)).toString();
        QString ori_file = model->data(index).toString();

        if(ori_file.isEmpty())
            dialog_file->setDirectory(target_dir);
        else
            dialog_file->setDirectory(ori_file.left(ori_file.lastIndexOf("/")));

        dialog_file->setMinimumSize(640, 480);
        dialog_file->setNameFilter("*.launch");
        dialog_file->setWindowTitle("Choose Launch File");
        dialog_file->setFileMode( QFileDialog::ExistingFile );

        connect(dialog_file,  &QFileDialog::directoryEntered, this, [=](const QString& dir) {
            if(dir.indexOf(target_dir) < 0)
                dialog_file->setDirectory(target_dir);
        });

        return dialog_file;
    }
}

void FileDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QString err_msg;
    if(index.column() == 0) {
        QLineEdit *line_edit = static_cast<QLineEdit *>(editor);
        QString ori_data = model->data(index).toString();
        QString new_data = line_edit->text().trimmed();

        if(tableview->isDataValid(index, new_data, err_msg)) {
            model->setData(index, new_data);
            tableview->addKey(new_data);
            if(!ori_data.isEmpty()) {
                tableview->deleteKey(ori_data);
            }

            tableview->key_changed = true;
        }
        else {
            QMessageBox::critical(line_edit, "Invalid Data", err_msg);
            model->setData(index, ori_data);
        }

        /*
        if(new_data.isEmpty()) {
            model->setData(index, ori_data);
            QMessageBox::critical(line_edit, "Empty Key", "Name should not be empty!");
        }
        else if(!tableview->isKeyReapted(new_data)) {
            QStandardItemModel *standard_model = static_cast<QStandardItemModel *>(model);
            QStandardItem *item = standard_model->itemFromIndex(index);
            item->setToolTip(new_data);

            model->setData(index, new_data);
            tableview->addKey(new_data);
            if(!ori_data.isEmpty()) {
                tableview->deleteKey(ori_data);
            }
            else {
                QFont key_font = item->font();
                key_font.setBold(true);
                item->setFont(key_font);
            }

            tableview->key_changed = true;
        }
        else if(new_data != ori_data){
            model->setData(index, ori_data);
            QMessageBox::critical(tableview, "Repeated Key", "Name should be unique!");
        }
        */
    }
    else if(index.column() < 3)
    {
        QFileDialog *dialog = static_cast<QFileDialog *>(editor);
        if(dialog->result() == QDialog::Accepted) {
            QStringList filename = dialog->selectedFiles();

            if(tableview->isDataValid(index, filename[0], err_msg)) {
                model->setData(index, filename[0]);
            }
            else {
                QString filename_ori = model->data(index).toString();
                model->setData(index, filename_ori);
                QMessageBox::critical(tableview, "Invalid Data", err_msg);
            }
        }
    }
    else{

    }
}

void FileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    viewOption.decorationAlignment = Qt::AlignCenter;
    viewOption.decorationPosition = QStyleOptionViewItem::Top;
    viewOption.decorationSize = QSize(20,20);
    QStyledItemDelegate::paint(painter, viewOption, index);

    if (index.column() > 2)
    {
        bool data = index.model()->data(index, Qt::UserRole).toBool();

        QStyleOptionButton check_box_style_option;
        check_box_style_option.state |= QStyle::State_Enabled;
        if(data)
        {
            check_box_style_option.state |= QStyle::State_On;
        }
        else
        {
            check_box_style_option.state |= QStyle::State_Off;
        }

        check_box_style_option.rect = QRect(option.rect.topLeft().x() + option.rect.width() / 2 - 4,
                                            option.rect.topLeft().y() + option.rect.height() / 2,
                                            0,
                                            0);
        QApplication::style()->drawControl(QStyle::CE_CheckBox,&check_box_style_option,painter);

    }
}

bool FileDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect checkbox_rect = QRect(option.rect.topLeft().x() + option.rect.width() / 2 - 10,
                                option.rect.topLeft().y() + option.rect.height() / 2 - 10,
                                20,
                                20);
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress && checkbox_rect.contains(mouseEvent->pos()))
    {
        if (index.column() > 2)
        {
            bool data = model->data(index, Qt::UserRole).toBool();
            model->setData(index, !data, Qt::UserRole);
        }
    }


    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
