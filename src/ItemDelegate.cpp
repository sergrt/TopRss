#include "ItemDelegate.h"

#include "ItemListModel.h"
#include "ItemRenderCompactWidget.h"
#include "ItemRenderWidget.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>

namespace {
ItemRenderer* makeRenderer(QWidget* parent, bool compact) {
    if (compact)
        return new ItemRenderCompactWidget(parent);
    else
        return new ItemRenderWidget(parent);
}
}  // namespace

ItemDelegate::ItemDelegate(QListView* parent, bool compact) : QStyledItemDelegate(parent), parentView_{parent} {
    renderer_ = std::unique_ptr<ItemRenderer>(makeRenderer(parentView_, compact));

    // Initial call to update layout prior to actual drawing
    renderer_->renderPixmap(QSize{renderer_->width(), renderer_->height()});
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (!index.isValid())
        return QStyledItemDelegate::sizeHint(option, index);

    const auto w = parentView_->contentsRect().width() - parentView_->verticalScrollBar()->rect().width() -
                   2 * parentView_->spacing();

    return {w, renderer_->height()};
}

void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                         const QModelIndex& index) const {
    prepareRenderer(index);

    // Check if link hovered and it is needed to be highlighted, and cursor changed
    bool highlightLink = false;
    if (option.state & QStyle::State_MouseOver) {
        auto linkRect = renderer_->linkRect();
        linkRect.moveTo(linkRect.left(), option.rect.top() + linkRect.top());
        highlightLink = linkRect.contains(hoverPosition_);
    } else {
        hoverPosition_ = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    }
    renderer_->setLinkHighlight(highlightLink);
    parentView_->setCursor(QCursor(highlightLink ? Qt::PointingHandCursor : Qt::ArrowCursor));

    const auto pixmap = renderer_->renderPixmap(option.rect.size());
    painter->drawPixmap(option.rect.topLeft(), pixmap);
}

void ItemDelegate::prepareRenderer(const QModelIndex& index) const {
    renderer_->setTitle(index.data(ItemListModel::Title).toString());
    renderer_->setDescription(index.data(ItemListModel::Description).toString());
    renderer_->setLink(index.data(ItemListModel::Link).toString());
    renderer_->setPubDate(index.data(ItemListModel::PubDate).toDateTime());
    renderer_->setCreator(index.data(ItemListModel::Creator).toString());
}

bool ItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                               const QModelIndex& index) {
    const auto event_type = event->type();
    if (event_type == QEvent::MouseButtonPress) {
        mousePressedIndex_ = index;  // To avoid handling release button on other item
    } else if (event_type == QEvent::MouseButtonRelease) {
        if (index == mousePressedIndex_) {
            auto linkRect = renderer_->linkRect();
            linkRect.moveTo(linkRect.left(), option.rect.top() + linkRect.top());
            const auto me = static_cast<QMouseEvent*>(event);
            if (linkRect.contains(me->pos())) {
                const auto url = index.data(ItemListModel::Link).toString();
                QDesktopServices::openUrl(url);
            }
        }
    } else if (event_type == QEvent::MouseMove) {
        hoverPosition_ = static_cast<QMouseEvent*>(event)->pos();
        return true;
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
