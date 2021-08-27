#pragma once
// Normal mode article rendering widget. Provides with title, date (in national format), author, description and
// clickable link

#include "ui_ItemRenderWidget.h"

#include "ItemRenderer.h"

class ItemRenderWidget final : public QWidget, public ItemRenderer {
    Q_OBJECT

public:
    explicit ItemRenderWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* event) override;

    void setTitle(const QString& title) override;
    void setDescription(const QString& description) override;
    void setLink(const QString& link) override;
    void setPubDate(const QDateTime& pubDate) override;
    void setCreator(const QString& creator) override;

    QPixmap renderPixmap(const QSize &size) override;

    QRect linkRect() const override;
    void setLinkHighlight(bool highlight) override;

    int height() const override;
    int width() const override;

private:
    Ui::ItemRenderWidget ui_;

    // Elided text should be be stored and applied on labels on actual painting.
    // Regular text might be applied immediately as widget will be in charge of displaying it
    QString title_;
    QString description_;
    QString link_;
};
