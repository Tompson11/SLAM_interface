#ifndef FILEDELEGATE_H
#define FILEDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QFileDialog>
#include <QItemDelegate>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QMouseEvent>
#include <QClipboard>
#include <iostream>
class LaunchTableView;

class FileDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    FileDelegate(QWidget *parent = nullptr, LaunchTableView *table_view = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    LaunchTableView *tableview;
};

#endif // FILEDELEGATE_H
