#include "launchtableview.h"
#include <iostream>

LaunchTableView::LaunchTableView(QWidget *parent) : QTableView(parent)
{
    model_ = new QStandardItemModel(this);
    select_model_ = new QItemSelectionModel(model_);

    this->setModel(model_);
    this->setSelectionModel(select_model_);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setItemDelegate(new FileDelegate(this, this));
    this->setFocusPolicy(Qt::StrongFocus);

    connect(model_, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

QString LaunchTableView::getIncompleteRowsInQString(QString seperator) {
    QString ret;
    for(auto it = incomplete_rows.begin(); it != incomplete_rows.end(); it++) {
        if(it != incomplete_rows.begin())
            ret += seperator;
        ret += QString::number(*it + 1);
    }
    return ret;
}

bool LaunchTableView::lookupIncompleteRow(int row, std::list<int>::iterator &it) {
    it = incomplete_rows.begin();

    for(; it != incomplete_rows.end(); it++) {
        if((*it) >= row) {
            break;
        }
    }

    if(it == incomplete_rows.end() || (*it) != row)
        return false;
    else
        return true;
}

void LaunchTableView::clearIncompleteRows() {
    while (incomplete_rows.size()) {
        deleteRow(*(incomplete_rows.begin()));
    }
}

void LaunchTableView::addIncompleteRow(int row, bool row_reduce) {
    std::list<int>::iterator it;
    lookupIncompleteRow(row, it);
    addIncompleteRow(row, row_reduce, it);
}

void LaunchTableView::addIncompleteRow(int row, bool row_reduce, std::list<int>::iterator &it) {
    std::list<int>::iterator it_new = incomplete_rows.insert(it, row);

    if(row_reduce) {
        it_new++;
        for(; it_new != incomplete_rows.end(); it_new++) {
            (*it_new) = (*it_new) + 1;
        }
    }
}

void LaunchTableView::deleteIncompleteRow(int row, bool row_reduce) {
    std::list<int>::iterator it;
    lookupIncompleteRow(row, it);
    deleteIncompleteRow(row, row_reduce, it);
}

void LaunchTableView::deleteIncompleteRow(int row, bool row_reduce, std::list<int>::iterator &it) {
    it = incomplete_rows.erase(it);

    if(row_reduce) {
        for(; it != incomplete_rows.end(); it++) {
            (*it) = (*it) - 1;
        }
    }
}

void LaunchTableView::addRow(int row, QList<QStandardItem*> &items) {
    if(row > model_->rowCount())
        row = model_->rowCount();

    model_->insertRow(row, items);

    if(items.size() && !items.front()->text().isEmpty()) {
        key_set.insert(items.front()->text().toStdString());
    }

    if(isRowIncomplete(row)) {
        addIncompleteRow(row, true);
    }
}

void LaunchTableView::deleteRow(int row) {
    if(row < model_->rowCount()) {
        if(isRowIncomplete(row)) {
            deleteIncompleteRow(row, true);
        }

        QString key = model_->data(model_->index(row, 0)).toString();
        key_set.erase(key.toStdString());

        model_->removeRow(row);
    }
}

void LaunchTableView::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
    for(int row = topLeft.row(); row <= bottomRight.row(); row++) {
        if(isRowIncomplete(row)) {
            std::list<int>::iterator it;
            if(!lookupIncompleteRow(row, it)) {
                addIncompleteRow(row, false, it);
            }
        }
        else {
            std::list<int>::iterator it;
            if(lookupIncompleteRow(row, it)) {
                deleteIncompleteRow(row, false, it);
            }
        }

        for(int col = topLeft.column(); col <= bottomRight.column(); col++) {
            QStandardItem *item = model_->itemFromIndex(model_->index(row, col));
            QString data = item->data(Qt::DisplayRole).toString();

            if(!data.isEmpty())
                item->setToolTip(data);

            if(col == 0 && item->font().bold() == false) {
                auto font = item->font();
                font.setBold(true);
                item->setFont(font);
            }
        }

    }
}

bool LaunchTableView::isKeyReapted(const QString &key) {
    return key_set.count(key.toStdString()) > 0;
}

bool LaunchTableView::deleteKey(const QString &key) {
    key_set.erase(key.toStdString());
    return true;
}

bool LaunchTableView::addKey(const QString &key) {
    key_set.insert(key.toStdString());
    return true;
}

QStringList LaunchTableView::getAllKeys() {
    QStringList ret;
    for(int i = 0; i < model_->rowCount(); i++) {
        ret << model_->data(model_->index(i, 0)).toString();
    }
    return ret;
}

void LaunchTableView::notifyUpdate() {
    emit launchTableUpdate();
}

bool LaunchTableView::existIncompleteRows() {
    return incomplete_rows.size() > 0;
}

bool LaunchTableView::isRowIncomplete(int row) {
    if(row >= model_->rowCount())
        return true;

    for(int col = 0; col < model_->columnCount(); col++) {
        if(model_->data(model_->index(row, col)).toString().size() == 0)
            return true;
    }

    return false;
}

void LaunchTableView::setColumnWidth(int col, int wid) {
    QTableView::setColumnWidth(col, wid);
}

void LaunchTableView::setRowHeight(int row, int hei) {
    QTableView::setRowHeight(row, hei);
}

bool LaunchTableView::isDataValid(const QModelIndex &index, const QVariant &data, QString &err_msg) {
    if(index.column() == 0) {
        QString key = data.toString().trimmed();

        if(key.isEmpty()) {
            err_msg = "Empty Name!";
            return false;
        }
        else if(isKeyReapted(key)) {
            err_msg = "Reapted Name!";
            return false;
        }
        else {
            return true;
        }
    }
    else if(index.column() == 1) {
        QUrl workspace{data.toString().trimmed()};
        if(workspace.isValid() == false) {
            err_msg = "Invalid directory format!";
            return false;
        }

        if(utils::existDir(workspace.toString() + "/devel/setup.bash")) {
            QUrl launch_file{model_->data(index.siblingAtColumn(2)).toString()};
            if(!launch_file.isEmpty() && !workspace.isParentOf(launch_file)) {
                err_msg = "Launch file is not located in the workspace!";
                return false;
            }

            return true;
        }
        else {
            err_msg = "Invalid ros workspace!";
            return false;
        }
    }
    else if(index.column() == 2) {
        QUrl launch_file{data.toString().trimmed()};
        if(launch_file.isValid() == false || !launch_file.fileName().endsWith(".launch")) {
            err_msg = "Invalid launch file format!";
            return false;
        }

        if(utils::existDir(launch_file.toString())) {
            QUrl workspace{model_->data(index.siblingAtColumn(1)).toString()};
            if(!workspace.isEmpty() && !workspace.isParentOf(launch_file)) {
                err_msg = "Launch file is not located in the previous workspace!";
                return false;
            }

            return true;
        }
        else {
            err_msg = "Non-existent launch file!";
            return false;
        }
    }

    return false;
}


void LaunchTableView::keyPressEvent( QKeyEvent* event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->matches(QKeySequence::Copy)){
           if (this->selectedIndexes().size() > 0) {
               auto index = this->selectedIndexes().front();
               QString text_to_copy = model_->data(index).toString();
               QApplication::clipboard()->setText(text_to_copy);
           }
        }
        else if(keyEvent->matches(QKeySequence::Paste)) {
           if (this->selectedIndexes().size() > 0) {
               auto index = this->selectedIndexes().front();
               QString text_to_paste = QApplication::clipboard()->text();

               QString err_msg;
               if(isDataValid(index, text_to_paste, err_msg))
                   model_->setData(index, text_to_paste);
               else
                   QMessageBox::critical(this, "Paste Error", QString("Paste content <font><b>[%1]</b> fail!<br> * %2</font>").arg(text_to_paste, err_msg));
           }
        }
    }
}
