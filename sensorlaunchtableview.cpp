#include "sensorlaunchtableview.h"

SensorLaunchTableView::SensorLaunchTableView(QWidget *parent) : LaunchTableView(parent)
{
    model_ = new QStandardItemModel(this);
    model_->setColumnCount(3);
    model_->setHeaderData(0, Qt::Horizontal, "Sensor Name");
    model_->setHeaderData(1, Qt::Horizontal, "Workspace");
    model_->setHeaderData(2, Qt::Horizontal, "Launch File");

    select_model_ = new QItemSelectionModel(model_);

    this->setModel(model_);
    this->setSelectionModel(select_model_);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setItemDelegate(new FileDelegate(this, this));

    connect(model_, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

bool SensorLaunchTableView::isRowIncomplete(int row) {
    if(row >= model_->rowCount())
        return true;

    for(int col = 0; col < 3; col++) {
        if(model_->data(model_->index(row, col)).toString().size() == 0)
            return true;
    }

    return false;
}

void SensorLaunchTableView::loadHistoryConfig() {
    QList<QStandardItem*> items;
    items << new QStandardItem("fast_lio") << new QStandardItem("/home/zhangzhuo/catkin_ws") << new QStandardItem("/home/zhangzhuo/catkin_ws/src/fast_lio/launch/mapping_velodyne.launch");
    this->addRow(0, items);
    notifyUpdate();
}
