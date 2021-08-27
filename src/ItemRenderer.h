#pragma once
// Interface for Rss item renderer. This allows us to have several renderers (there are two of them in this example -
// compact and "regular"), and any of these renderers requires no changes to item delegate

#include <QRect>
#include <QPixmap>

class ItemRenderer {
public:
    virtual ~ItemRenderer() = default;

    virtual void setTitle(const QString& title) {}
    virtual void setDescription(const QString& description) {}
    virtual void setLink(const QString& link) {}
    virtual void setPubDate(const QDateTime& pubDate){}
    virtual void setCreator(const QString& creator) {}

    virtual QPixmap renderPixmap(const QSize& size) = 0;

    virtual QRect linkRect() const = 0;
    virtual void setLinkHighlight(bool highlight) {}

    virtual int height() const = 0;
    virtual int width() const = 0;
};
