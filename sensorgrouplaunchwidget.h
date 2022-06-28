#ifndef SENSORGROUPLAUNCHWIDGET_H
#define SENSORGROUPLAUNCHWIDGET_H

#include "grouplaunchwidget.h"
#include <QGridLayout>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include "sensorwidget.h"
#include "components/qtmaterialraisedbutton.h"

class SensorGroupLaunchWidget : public GroupLaunchWidget
{
    Q_OBJECT
public:
    explicit SensorGroupLaunchWidget(QWidget *parent = nullptr);

protected:
    virtual LaunchWidget* createAddedWidget();

private:
    QDialog *dialog_;

    QGridLayout *layout_dialog_;
    QLabel *label_name_in_dialog_;
    QLabel *label_type_in_dialog_;
    QComboBox *combo_in_dialog_;
    QLineEdit *edit_in_dialog_;
    QtMaterialRaisedButton *button_in_dialog_;

private slots:
    void onButtonCreateClicked();


};

#endif // SENSORGROUPLAUNCHWIDGET_H
