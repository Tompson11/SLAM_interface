#include "launchfilereader.h"
#include <iostream>
LaunchFileReader::LaunchFileReader(const QString &file_name, QStandardItemModel *model) :
    file_name_(file_name), model_(model)
{

}

bool LaunchFileReader::readAndBuildModel(QString &err_msg) {
    if(model_ == nullptr) {
        return false;
    }

    QFile file(file_name_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        err_msg = QString("cannot open file [%1]").arg(file_name_);
        return false;
    }

    pReader_ = new QXmlStreamReader(&file);
    while (!pReader_->isStartElement())
    {
        pReader_->readNext();
    }

    if(pReader_->isStartElement() && pReader_->name() != "launch") {
        err_msg = QString("Launch file should begin with <launch>");
        return false;
    }

    pReader_->readNext();

    arg_header_item_ = new QStandardItem("[Args]");
    arg_header_item_->setForeground(QBrush(QColor(255, 0, 0)));
    auto font = arg_header_item_->font();
    font.setBold(true);
    arg_header_item_->setFont(font);
    model_->appendRow(arg_header_item_);
    model_->itemFromIndex(arg_header_item_->index().siblingAtColumn(3))->setEditable(false);

    param_header_item_ = new QStandardItem("[Params]");
    param_header_item_->setFont(font);
    param_header_item_->setForeground(QBrush(QColor(0, 0, 255)));
    model_->appendRow(param_header_item_);
    model_->itemFromIndex(param_header_item_->index().siblingAtColumn(3))->setEditable(false);

    return read("/", err_msg);
}

bool LaunchFileReader::read(const QString &base_ns, QString &err_msg) {
    while (!pReader_->atEnd())
    {
        if (pReader_->isStartElement())
        {
            if(pReader_->name() == "param") {
                if(!readLabelParam(base_ns, err_msg))
                    return false;
            }
            else if(pReader_->name() == "node") {
                if(!readLabelNode(base_ns, err_msg))
                    return false;
            }
            else if(pReader_->name() == "group") {
                if(!readLabelGroup(base_ns, err_msg))
                    return false;
            }
            else if(pReader_->name() == "arg") {
                if(!readLabelArg(base_ns, err_msg))
                    return false;
            }

            pReader_->readNext();
        }
        else {
            pReader_->readNext();
        }
    }

    return true;
}

bool LaunchFileReader::readLabelParam(const QString &base_ns, QString &err_msg) {
    if(namespace_item_map_.count(base_ns.toStdString()) == 0){
        QStandardItem *ns_item = new QStandardItem(base_ns);
        ns_item->setToolTip(base_ns);
        ns_item->setEditable(false);
        param_header_item_->appendRow(ns_item);
        param_header_item_->setChild(ns_item->row(), 4, new QStandardItem(""));
        namespace_item_map_.emplace(base_ns.toStdString(), ns_item);
    }

    QStandardItem *ns_item = namespace_item_map_.at(base_ns.toStdString());

    QXmlStreamAttributes attributes = pReader_->attributes();
    if (attributes.hasAttribute("name") && attributes.hasAttribute("value"))
    {
        QStandardItem *param_item = new QStandardItem("");
        ns_item->appendRow(param_item);

        QStandardItem *param_name_item = new QStandardItem(attributes.value("name").toString());
        ns_item->setChild(param_item->row(), 1, param_name_item);
        param_name_item->setToolTip(attributes.value("name").toString());

        if(attributes.hasAttribute("type")) {
            QStandardItem *param_type_item = new QStandardItem(attributes.value("type").toString());
            ns_item->setChild(param_item->row(), 2, param_type_item);
            param_type_item->setToolTip(attributes.value("type").toString());
        }
        else {
            QStandardItem *param_type_item = new QStandardItem("");
            ns_item->setChild(param_item->row(), 2, param_type_item);
        }

        QStandardItem *param_value_item = new QStandardItem(attributes.value("value").toString());
        param_value_item->setData(attributes.value("value").toString(), Qt::UserRole);
        ns_item->setChild(param_item->row(), 3, param_value_item);
        param_value_item->setToolTip(attributes.value("value").toString());

        ns_item->setChild(param_item->row(), 4, new QStandardItem(""));

        return true;
    }
    else {
        return false;
    }
}

bool LaunchFileReader::readLabelNode(const QString &base_ns, QString &err_msg) {
    QString ns = base_ns;
    QXmlStreamAttributes attributes = pReader_->attributes();

    if(attributes.hasAttribute("ns")) {
        if(ns != "/")
            ns += "/" + attributes.value("ns");
        else
            ns += attributes.value("ns");
    }

    if(attributes.hasAttribute("name")) {
        if(ns != "/")
            ns += "/" + attributes.value("name");
        else
            ns += attributes.value("name");
    }
    else {
        return false;
    }

    pReader_->readNext();
    return read(ns, err_msg);
}

bool LaunchFileReader::readLabelGroup(const QString &base_ns, QString &err_msg) {
    QString ns = base_ns;
    QXmlStreamAttributes attributes = pReader_->attributes();

    if(attributes.hasAttribute("ns")) {
        if(ns != "/")
            ns += "/" + attributes.value("ns");
        else
            ns += attributes.value("ns");
    }

    pReader_->readNext();
    return read(ns, err_msg);
}

bool LaunchFileReader::readLabelArg(const QString &base_ns, QString &err_msg) {
    if(base_ns != "/")
        return true;

    QXmlStreamAttributes attributes = pReader_->attributes();

    if(attributes.hasAttribute("name")) {
        if(attributes.hasAttribute("value") && attributes.hasAttribute("default")) {
            return false;
        }

        QStandardItem *arg_item = new QStandardItem("");
        arg_header_item_->appendRow(arg_item);

        QStandardItem *arg_name_item = new QStandardItem(attributes.value("name").toString());
        arg_header_item_->setChild(arg_item->row(), 1, arg_name_item);
        arg_name_item->setToolTip(attributes.value("name").toString());

        if(attributes.hasAttribute("default")) {
            arg_header_item_->setChild(arg_item->row(), 2, new QStandardItem("default"));

            QStandardItem *arg_value_item = new QStandardItem(attributes.value("default").toString());
            arg_value_item->setData(attributes.value("default").toString(), Qt::UserRole);
            arg_header_item_->setChild(arg_item->row(), 3, arg_value_item);
            arg_value_item->setToolTip(attributes.value("default").toString());
        }
        else {
            arg_header_item_->setChild(arg_item->row(), 2, new QStandardItem("value"));

            QStandardItem *arg_value_item = new QStandardItem(attributes.value("value").toString());
            arg_value_item->setData(attributes.value("value").toString(), Qt::UserRole);
            arg_header_item_->setChild(arg_item->row(), 3, arg_value_item);
            arg_value_item->setToolTip(attributes.value("value").toString());
        }

        arg_header_item_->setChild(arg_item->row(), 4, new QStandardItem(""));

        return true;
    }
    else {
        return false;
    }


}
