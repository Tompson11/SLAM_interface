#include "launchtableview.h"
#include "filedelegate.h"
#include "utils/sys.h"
#include <iostream>

FileDelegate::FileDelegate(QWidget *parent, LaunchTableView *table_view): QItemDelegate(parent), tableview(table_view)
{

}


QWidget* FileDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if(index.column() == 0) {
        QLineEdit *line_edit_name = new QLineEdit(parent);
        return line_edit_name;
    }
    else if(index.column() == 1){
        QFileDialog *dialog_dir = new QFileDialog(parent);
        dialog_dir->setWindowTitle("Choose Workspace Directory");
        dialog_dir->setFileMode( QFileDialog::DirectoryOnly );
        return dialog_dir;
    }
    else if(index.column() == 2){
        QFileDialog *dialog_file = new QFileDialog(parent);
        auto *model = tableview->model();
        QString target_dir = model->data(model->index(index.row(), index.column() - 1)).toString();
        dialog_file->setDirectory(target_dir);
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
    if(index.column() > 0) {
        QFileDialog *dialog = static_cast<QFileDialog *>(editor);
        if(dialog->result() == QDialog::Accepted) {
            QStringList filename = dialog->selectedFiles();

            if(index.column() == 1) {
                if(utils::existDir(filename[0] + "/devel/setup.bash")) {
                    model->setData(index, filename[0]);
                }
                else {
                    QString filename_ori = model->data(index).toString();
                    model->setData(index, filename_ori);
                    QMessageBox::critical(tableview, "Invalid workspace path", "Please select valid ROS workspace path!");
                }
            }
            else {
                model->setData(index, filename[0]);
            }
        }
    }
    else{
        QLineEdit *line_edit = static_cast<QLineEdit *>(editor);
        QString ori_data = model->data(index).toString();
        QString new_data = line_edit->text().trimmed();

        if(new_data.isEmpty()) {
            model->setData(index, ori_data);
            QMessageBox::critical(line_edit, "Empty Key", "Name should not be empty!");
        }
        else if(!tableview->isKeyReapted(new_data)) {
            model->setData(index, new_data);
            tableview->addKey(new_data);
            if(!ori_data.isEmpty()) {
                tableview->deleteKey(ori_data);
            }

            tableview->key_changed = true;
        }
        else {
            model->setData(index, ori_data);
            QMessageBox::critical(tableview, "Repeated Key", "Name should be unique!");
        }
    }
}
