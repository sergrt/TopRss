#include "ItemListModel.h"

int ItemListModel::rowCount(const QModelIndex& parent) const {
    return static_cast<int>(data_.size());
}

QVariant ItemListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return {};

    const auto idx = static_cast<size_t>(index.row());
    if (idx >= data_.size())
        return {};

    const auto& item = data_[idx];

    if (role == Role::Title) {
        return item.title();
    } else if (role == Role::Description) {
        return item.description();
    } else if (role == Role::Link) {
        return item.link();
    } else if (role == Role::Creator) {
        return item.creator();
    } else if (role == Role::PubDate) {
        return item.pubDate();
    } else if (role == Qt::ToolTipRole) {
        return item.title();
    }

    return {};
}

void ItemListModel::setRssData(std::vector<RssItem> data) {
    beginResetModel();
    data_ = std::move(data);
    endResetModel();
}

void ItemListModel::clearRssData() {
    beginResetModel();
    data_.clear();
    endResetModel();
}
