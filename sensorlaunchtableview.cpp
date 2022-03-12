#include "sensorlaunchtableview.h"

SensorLaunchTableView::SensorLaunchTableView(QWidget *parent) : LaunchTableView(parent)
{
    model_->setColumnCount(3);
    model_->setHeaderData(0, Qt::Horizontal, "Sensor Name");
    model_->setHeaderData(1, Qt::Horizontal, "Workspace");
    model_->setHeaderData(2, Qt::Horizontal, "Launch File");
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
