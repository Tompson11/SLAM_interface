#ifndef GROUPCMDWIDGET_H
#define GROUPCMDWIDGET_H

#include <QWidget>
#include <QListView>
#include <QListWidget>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFrame>
#include <QSettings>

#include "cmddelegate.h"
#include "cmdwidget.h"
#include "components/qtmaterialscrollbar.h"

class GroupCmdWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupCmdWidget(QWidget *parent = nullptr);
    void saveCurrentConfig(QSettings *settings, const QString &group);
    void loadConfig(QSettings *settings, const QString &group);
    void toggleCompactLayout();
    void refreshDisplay();
    void setRoscoreWidget(RoscoreWidget *wid);

private:
    void addCmdWidget(CmdWidget *new_widget, int row = -1, const QString &cmd_name = "", const QString &cmd_code = "");

    RoscoreWidget *roscore_widget_ = nullptr;

    QLabel *label_title_;
    QLabel *label_title_back_;

    QListWidget *list_widget_;

    QtMaterialScrollBar *scrollbar_hor_for_list_widget;
    QtMaterialScrollBar *scrollbar_vert_for_list_widget;

    QStandardItemModel *model_;
    QItemSelectionModel *select_model_;

    QFrame *frame_;

    QGridLayout *layout_;
    QGridLayout *layout_main_;

    QFrame *button_frame_;
    QtMaterialRaisedButton *button_add_;

    bool use_compact_layout_ = false;

    std::unordered_map<CmdWidget*, QListWidgetItem*> m_cmd_to_list_;
signals:

protected slots:
    void onButtonAddClicked();
    void onCmdWidgetResized(CmdWidget *cmd_wid);
    void onCmdWidgetDeleted(CmdWidget *cmd_wid);
    void onCmdWidgetChangePosition(CmdWidget* w_from, CmdWidget* w_to);
    void onCmdWidgetDraged();
};

#endif // GROUPCMDWIDGET_H
