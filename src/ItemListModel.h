#pragma once
// Simple model to be used with list displaying RSS items

#include "RssItem.h"

#include <QAbstractListModel>

class ItemListModel final : public QAbstractListModel {
public:
    enum Role {
        Title = Qt::UserRole + 1,
        Description,
        Link,
        Creator,
        PubDate
    };

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void setRssData(std::vector<RssItem> data);
    void clearRssData();

private:
    std::vector<RssItem> data_;
};
