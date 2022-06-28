#include "slamlaunchtableview.h"

SlamLaunchTableView::SlamLaunchTableView(QWidget *parent) : LaunchTableView(parent)
{
    model_->setColumnCount(7);
    model_->setHeaderData(0, Qt::Horizontal, "Program Name");
    model_->setHeaderData(1, Qt::Horizontal, "Workspace");
    model_->setHeaderData(2, Qt::Horizontal, "Launch File");
    model_->setHeaderData(3, Qt::Horizontal, "LiDAR");
    model_->setHeaderData(4, Qt::Horizontal, "Camera");
    model_->setHeaderData(5, Qt::Horizontal, "IMU");
    model_->setHeaderData(6, Qt::Horizontal, "Others");
}

bool SlamLaunchTableView::isRowIncomplete(int row) {
    if(row >= model_->rowCount())
        return true;

    for(int col = 0; col < 3; col++) {
        if(model_->data(model_->index(row, col)).toString().size() == 0)
            return true;
    }

    for(int col = 3; col < model_->columnCount(); col++) {
        if(model_->data(model_->index(row, col), Qt::UserRole).toBool())
            return false;
    }

    return true;
}
