#ifndef LAUNCHTABLEVIEW_H
#define LAUNCHTABLEVIEW_H

#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "filedelegate.h"
#include <unordered_set>

class LaunchTableView: public QTableView
{
    Q_OBJECT

public:
    bool key_changed = false;

    explicit LaunchTableView(QWidget *parent = nullptr);
    virtual void loadHistoryConfig() = 0;
    bool existIncompleteRows();
    bool isKeyReapted(const QString &key);
    bool deleteKey(const QString &key);
    bool addKey(const QString &key);
    QStringList getAllKeys();
    void clearIncompleteRows();
    QString getIncompleteRowsInQString(QString seperator = ",");
    void addRow(int row, QList<QStandardItem*> &items);
    void deleteRow(int row);
    void notifyUpdate();

protected:
    virtual bool isRowIncomplete(int row) = 0;

    bool lookupIncompleteRow(int row, std::list<int>::iterator &it);
    void addIncompleteRow(int row, bool row_reduce);
    void addIncompleteRow(int row, bool row_reduce, std::list<int>::iterator &it);
    void deleteIncompleteRow(int row, bool row_reduce);
    void deleteIncompleteRow(int row, bool row_reduce, std::list<int>::iterator &it);

    std::list<int> incomplete_rows;
    std::unordered_set<std::string> key_set;

    QStandardItemModel *model_;
    QItemSelectionModel *select_model_;

private slots:
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());

signals:
    void launchTableUpdate();

};

#endif // LAUNCHTABLEVIEW_H
