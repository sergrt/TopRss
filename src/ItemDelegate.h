#pragma once
// Item delegate to be used to render items into Rss list.
// Delegate uses regular widgets for painting - this provides us with luxury of using Qt Designer, designers'
// blueprints and other regular workflow without need of manually creating/sizing/placing widgets. ItemDelegate
// manipulates with regular and compact rendering widgets via ItemRenderer interface, so it should be quite easy to
// extend view modes.

#include "ItemRenderer.h"

#include <QListView>
#include <QStyledItemDelegate>

#include <memory>
#include <numeric>

class ItemDelegate final : public QStyledItemDelegate {
    Q_OBJECT

public:
    ItemDelegate(QListView* parent, bool compact = false);

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;

private:
    void prepareRenderer(const QModelIndex& index) const;

    QModelIndex mousePressedIndex_;
    mutable QPoint hoverPosition_{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    std::unique_ptr<ItemRenderer> renderer_;
    QListView* parentView_{nullptr};
};