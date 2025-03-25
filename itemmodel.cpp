//
//  itemmodel.cpp
//  loveBus
//
//  Created by zf on 2025/3/10.
//

#include "itemmodel.h"
#include <QThread>

Item::Item(const QString &name) : m_name(name) {}

QString Item::name() const {
    return m_name;
}

ItemModel::ItemModel(QObject *parent) : QAbstractListModel(parent) {}// 从 QList<Item> 设置数据

void ItemModel::setDataFromList(const QList<Item> &items) {
    beginResetModel();
    m_items = items;
//    for (const auto &item : m_items) {
//            qDebug() << "Item.name =" << item.name();
//        }
    endResetModel();
}


int ItemModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_items.count(); // m_items.count() 返回 qsizetype 类型
}

QVariant ItemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return QVariant();

    const Item &item = m_items.at(index.row());
    if (role == NameRole)
        return item.name();
    
    return QVariant();
}

QHash<int, QByteArray> ItemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
