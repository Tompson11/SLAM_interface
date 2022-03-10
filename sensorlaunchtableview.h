#ifndef SENSORLAUNCHTABLEVIEW_H
#define SENSORLAUNCHTABLEVIEW_H

#include "launchtableview.h"
#include <QObject>
#include <QWidget>

class SensorLaunchTableView : public LaunchTableView
{
    Q_OBJECT
public:
    SensorLaunchTableView(QWidget *parent);
    virtual void loadHistoryConfig();

protected:
    bool isRowIncomplete(int row) override;
};

#endif // SENSORLAUNCHTABLEVIEW_H
