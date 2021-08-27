#include "ItemRenderCompactWidget.h"

#include "TextHelpers.h"

#include <QPainter>
#include <QStyle>

constexpr auto highlightedLinkQss = "color: #0000FF;";
constexpr auto regularLinkQss = "color: #000000;";

ItemRenderCompactWidget::ItemRenderCompactWidget(QWidget* parent) : QWidget(parent) {
    ui_.setupUi(this);
    hide();
}

QPixmap ItemRenderCompactWidget::renderPixmap(const QSize& size) {
    resize(size);
    return grab();
}

void ItemRenderCompactWidget::setTitle(const QString& title) {
    title_ = title;
}

void ItemRenderCompactWidget::paintEvent(QPaintEvent* event) {
    ui_.title->setText(elidedText(*ui_.title, title_));
    QWidget::paintEvent(event);
}

QRect ItemRenderCompactWidget::linkRect() const {
    // Compact view uses title as clickable link, so here we return title geometry
    auto res = ui_.title->rect();
    res.moveTo(ui_.title->pos());
    return res;
}

void ItemRenderCompactWidget::setLinkHighlight(bool highlight) {
    // Compact view uses title as clickable link, so here we manipulate with title stylesheet
    ui_.title->setStyleSheet(highlight ? highlightedLinkQss : regularLinkQss);
}

int ItemRenderCompactWidget::height() const {
    return QWidget::height();
}

int ItemRenderCompactWidget::width() const {
    return QWidget::width();
}
