#ifndef LAUNCHPARAMDELEGATE_H
#define LAUNCHPARAMDELEGATE_H

#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>

class LaunchParamDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    enum TableColumnMeaning {
        PARAMETER_NAMESPACE = 0,
        PARAMETER_NAME,
        PARAMETER_TYPE,
        PARAMETER_VALUE,
        PARAMETER_RESET
    };

public:
    LaunchParamDelegate(QWidget *parent = nullptr, QAbstractItemModel *model = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

private:
    QStandardItemModel *model_;

    void resetParamsOAllNamespace(QStandardItem *ns_header_item);
    void resetParamsOfCertainNamespace(QStandardItem *ns_header_item);

};

#endif // LAUNCHPARAMDELEGATE_H
