#ifndef FILEDELEGATE_H
#define FILEDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QItemDelegate>
#include <QLineEdit>
#include <QMessageBox>

class LaunchTableView;

class FileDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    FileDelegate(QWidget *parent = nullptr, LaunchTableView *table_view = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    LaunchTableView *tableview;
};

#endif // FILEDELEGATE_H
