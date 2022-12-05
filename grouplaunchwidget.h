#ifndef GROUPLAUNCHWIDGET_H
#define GROUPLAUNCHWIDGET_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include "components/qtmaterialraisedbutton.h"
#include "sensorwidget.h"
#include "slamwidget.h"

class GroupLaunchWidget : public QWidget
{
    Q_OBJECT
public:
    enum ModuleType{
        MODULE_SENSOR = 0b1,
        MODULE_SLAM = 0b10,
        MODULE_TOOL = 0b100,
    };

    explicit GroupLaunchWidget(QWidget *parent = nullptr, const QString &group_name = "", int allowed_module = MODULE_SENSOR | MODULE_SLAM | MODULE_TOOL);
    int getWidgetIndex(QWidget *wid);
    void appendWidget(LaunchWidget *wid);
    void clearWidgets();
    void toggleCompactLayout();
    int getCompactHeight();

protected:
    QGridLayout *main_layout_;
    QHBoxLayout *group_layout_;
    QVBoxLayout *group_layout_compact_;

    QtMaterialRaisedButton *add_button_;

    QDialog *dialog_;
    QGridLayout *layout_dialog_;
    QLabel *label_module_in_dialog_;
    QLabel *label_name_in_dialog_;
    QLabel *label_type_in_dialog_;
    QComboBox *combo_module_in_dialog_;
    QComboBox *combo_type_in_dialog_;
    QLineEdit *edit_in_dialog_;
    QtMaterialRaisedButton *button_in_dialog_;

    bool use_compact_layout_ = false;

    bool to_create_widget_ = false;
    int allowed_module_;

    LaunchWidget* createAddedWidget();

protected slots:
    void removeWidget(QWidget* w);
    void bubbleChangeWidgetPosition(QWidget* w_from, QWidget* w_to);
    void onButtonAddClicked();
    void onButtonCreateClicked();
    void onModuleChanged(const QString &module_name);

signals:
    void launchWidgetAdded(LaunchWidget*);
    void launchWidgetRemoved(LaunchWidget*);
};

#endif // GROUPLAUNCHWIDGET_H
