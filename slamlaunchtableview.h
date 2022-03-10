#ifndef SLAMLAUNCHTABLEVIEW_H
#define SLAMLAUNCHTABLEVIEW_H

#include "launchtableview.h"
#include <QObject>
#include <QWidget>

class SlamLaunchTableView : public LaunchTableView
{
    Q_OBJECT
public:
    SlamLaunchTableView(QWidget *parent);
    virtual void loadHistoryConfig();

protected:
    bool isRowIncomplete(int row) override;
};

#endif // SLAMLAUNCHTABLEVIEW_H
