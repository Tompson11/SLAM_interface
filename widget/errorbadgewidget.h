#ifndef ERRORBADGEWIDGET_H
#define ERRORBADGEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QGridLayout>
#include "clickablebadge.h"
#include "components/qtmaterialfab.h"
#include "components/qtmaterialflatbutton.h"
#include "components/qtmaterialavatar.h"
#include "components/qtmaterialraisedbutton.h"

class ErrorBadgeWidget : public QWidget
{
    Q_OBJECT

    const QString MSG_RED = QString("<font color = %1>").arg("#e74c3c");
    const QString MSG_GREEN = QString("<font color = %1>").arg("#27ae60");
    const QString MSG_YELLOW = QString("<font color = %1>").arg("#D4AC0D");
    const QString MSG_BLUE = QString("<font color = %1>").arg("#3498DB");
    const QString MSG_MAGENTA = QString("<font color = %1>").arg("#DA15AF");
    const QString MSG_CYAN = QString("<font color = %1>").arg("#15cdda");

public:
    explicit ErrorBadgeWidget(QWidget *parent = nullptr);
    void setBadgeParent(QWidget *parent = nullptr);
    void setBadgeShift(int shift_x, int shift_y);
    void appendMsg(QString msg);

private:

    size_t unread_msg_nbr = 0;

    clickableBadge *badge;

    QDialog *dialog_msg;
    QTextEdit *textedit_in_dialog;
    QtMaterialRaisedButton *button_clear_in_dialog;

    QGridLayout *layout_dialog;

signals:

public slots:
    void onBadgeClicked();
    void onButtonClearClicked();

};

#endif // ERRORBADGEWIDGET_H
