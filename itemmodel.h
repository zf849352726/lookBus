//
//  itemmodel.h
//  loveBus
//
//  Created by zf on 2025/3/10.
//

#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QList>


class Item {
public:
    Item(const QString &name);
    Item(const Item &other) = default;  // 允许默认拷贝
    QString name() const;

private:
    QString m_name;
};

class ItemModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Role {
        NameRole = Qt::UserRole + 1
    };

    explicit ItemModel(QObject *parent = nullptr);

    // 从 QList<Item> 设置数据
    void setDataFromList(const QList<Item> &items);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Item> m_items;
};

#endif // ITEMMODEL_H
