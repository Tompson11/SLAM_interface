#ifndef LAUNCHFILEREADER_H
#define LAUNCHFILEREADER_H

#include <unordered_map>
#include <QObject>
#include <QWidget>
#include <QStandardItemModel>
#include <QXmlStreamReader>
#include <QFile>

class LaunchFileReader
{
public:
    LaunchFileReader(const QString &file_name, QStandardItemModel *model);
    bool readAndBuildModel(QString &err_msg);

private:
    bool read(const QString &base_ns, QString &err_msg);
    bool readLabelParam(const QString &base_ns, QString &err_msg);
    bool readLabelNode(const QString &base_ns, QString &err_msg);
    bool readLabelGroup(const QString &base_ns, QString &err_msg);
    bool readLabelArg(const QString &base_ns, QString &err_msg);

    QString file_name_;
    QStandardItemModel *model_ = nullptr;
    QXmlStreamReader *pReader_ = nullptr;
    QStandardItem *arg_header_item_ = nullptr;
    QStandardItem *param_header_item_ = nullptr;

    std::unordered_map<std::string, QStandardItem*> namespace_item_map_;
};

#endif // LAUNCHFILEREADER_H
