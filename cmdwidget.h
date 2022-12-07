#ifndef CMDWIDGET_H
#define CMDWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QMouseEvent>
#include <QGridLayout>
#include <QLabel>

#include "utils/shellpool.h"
#include "components/qtmaterialraisedbutton.h"
#include "components/lib/qtmaterialstyle.h"
#include "components/qtmaterialcheckbox.h"

class CmdWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CmdWidget(QWidget *parent = nullptr);
    void setCmdName(const QString &name);
    void setCmdCode(const QString &cmd);
    QString getCmdName();
    QString getCmdCode();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    QtMaterialRaisedButton *button_cmd_;
    QtMaterialRaisedButton *button_delete_;

    QDialog *dialog_rename_;
    QLineEdit *lineedit_in_dialog_rename_;

    QDialog *dialog_cmd_;
    QTextEdit *textedit_in_dialog_cmd_;
    QtMaterialRaisedButton *button_clear_in_dialog_cmd_;
    QtMaterialCheckBox *checkbox_in_dialog_cmd_;

    QGridLayout *layout_main_;
    QGridLayout *layout_dialog_rename_;
    QGridLayout *layout_dialog_cmd_;

    QProcess *process_cmd_ = nullptr;

signals:
    void resized(CmdWidget *);
    void deleted(CmdWidget *);

protected slots:
    void onButtonCmdClicked();
    void onButtonDeleteClicked();
    void onButtonClearClicked();
    void onChecked(bool tog);

};

#endif // CMDWIDGET_H
