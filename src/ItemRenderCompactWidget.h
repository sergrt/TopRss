#pragma once
// Compact view render widget. Shows only title, which is also a clickable link.

#include "ui_ItemRenderCompactWidget.h"

#include "ItemRenderer.h"

class ItemRenderCompactWidget final : public QWidget, public ItemRenderer {
    Q_OBJECT

public:
    explicit ItemRenderCompactWidget(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

    void setTitle(const QString& title) override;

    QPixmap renderPixmap(const QSize& size) override;

    QRect linkRect() const override;
    void setLinkHighlight(bool highlight) override;

    int height() const override;
    int width() const override;

private:
    Ui::ItemRenderCompactWidget ui_;

    // Elided text should be be stored and applied on labels on actual painting.
    // Regular text might be applied immediately as widget will be in charge of displaying it
    QString title_;
};
