#include "ItemRenderWidget.h"

#include "TextHelpers.h"

#include <QDateTime>
#include <QPainter>
#include <QTextDocument>
#include <QTextLayout>

constexpr auto highlightedLinkQss = "color: #9999FF;";
constexpr auto regularLinkQss = "color: #0000FF;";

ItemRenderWidget::ItemRenderWidget(QWidget* parent) : QWidget(parent) {
    ui_.setupUi(this);
    hide();
}

QPixmap ItemRenderWidget::renderPixmap(const QSize& size) {
    resize(size);
    return grab();
}

void ItemRenderWidget::setTitle(const QString& title) {
    title_ = title;
}

void ItemRenderWidget::setDescription(const QString& description) {
    // QTextDocument allows us to get nice plain text from HTML
    // (note that RSS standard allows HTML to be embedded into "description"
    QTextDocument doc;
    doc.setHtml(description);
    description_ = doc.toPlainText();
}

void ItemRenderWidget::paintEvent(QPaintEvent* event) {
    ui_.title->setText(elidedText(*ui_.title, title_));
    ui_.link->setText(elidedText(*ui_.link, link_));

    QPainter painter(this);

    // Paint elided description
    QRect descrRect = ui_.description->rect();
    const QPoint descrPos = ui_.description->pos();
    descrRect.moveTo(descrPos.x(), descrPos.y());

    const auto fontMetrics = painter.fontMetrics();
    const int lineSpacing = fontMetrics.lineSpacing();
    const int x = descrPos.x();
    int y = descrPos.y();
    const auto availableWidth = descrRect.width();
    const auto availableHeight = descrRect.height();

    QTextLayout textLayout(description_, painter.font());
    textLayout.beginLayout();
    forever {
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;

        line.setLineWidth(availableWidth);
        const int nextLineY = y + lineSpacing;

        if (descrPos.y() + availableHeight >= nextLineY + lineSpacing) {
            line.draw(&painter, QPoint(x, y));
            y = nextLineY;
        } else {
            const QString lastLine = description_.mid(line.textStart());
            const QString elidedLastLine = fontMetrics.elidedText(lastLine, Qt::ElideRight, availableWidth);
            painter.drawText(QPoint(x, y + fontMetrics.ascent()), elidedLastLine);
            line = textLayout.createLine();
            break;
        }
    }
    textLayout.endLayout();

    QWidget::paintEvent(event);
}

void ItemRenderWidget::setLink(const QString& link) {
    link_ = tr("Read more: ") + link;
}

void ItemRenderWidget::setCreator(const QString& creator) {
    ui_.creator->setText(creator);
}

void ItemRenderWidget::setPubDate(const QDateTime& pubDate) {
    ui_.pubDate->setText(pubDate.toString());
}

QRect ItemRenderWidget::linkRect() const {
    auto res = ui_.link->rect();
    res.moveTo(ui_.link->pos());
    return res;
}

void ItemRenderWidget::setLinkHighlight(bool highlight) {
    ui_.link->setStyleSheet(highlight ? highlightedLinkQss : regularLinkQss);
}

int ItemRenderWidget::height() const {
    return QWidget::height();
}

int ItemRenderWidget::width() const {
    return QWidget::width();
}
