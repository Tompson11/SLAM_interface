#ifndef CMDDELEGATE_H
#define CMDDELEGATE_H
#include <iostream>

#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QDialog>

class CmdDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CmdDelegate(QWidget *parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QTimer* timer_;

private slots:
    void execCmd();
};

#endif // CMDDELEGATE_H
