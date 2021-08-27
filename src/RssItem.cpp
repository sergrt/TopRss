#include "RssItem.h"

#include <stdexcept>

RssItem::RssItemBuilder& RssItem::RssItemBuilder::with_title(const QString& title) {
    properties_.title = title;
    return *this;
}

RssItem::RssItemBuilder& RssItem::RssItemBuilder::with_description(const QString& description) {
    properties_.description = description;
    return *this;
}

RssItem::RssItemBuilder& RssItem::RssItemBuilder::with_link(const QString& link) {
    properties_.link = link;
    return *this;
}

RssItem::RssItemBuilder& RssItem::RssItemBuilder::with_pubDate(const QDateTime& pubDate) {
    properties_.pubDate = pubDate;
    return *this;
}

RssItem::RssItemBuilder& RssItem::RssItemBuilder::with_creator(const QString& creator) {
    properties_.creator = creator;
    return *this;
}

RssItem RssItem::RssItemBuilder::build() const {
    return RssItem(properties_);
}

////////////////

RssItem::RssItem(const RssItemProperties& properties) {
    if (properties.description.isEmpty() &&
        properties.title.isEmpty())  // RSS specification requires at least one of these to be present
        throw std::runtime_error("Invalid RSS item properties set");

    title_ = properties.title;
    description_ = properties.description;
    link_ = properties.link;
    pubDate_ = properties.pubDate;
    creator_ = properties.creator;
}

QString RssItem::title() const {
    return title_;
}

QString RssItem::description() const {
    return description_;
}

QString RssItem::link() const {
    return link_;
}

QDateTime RssItem::pubDate() const {
    return pubDate_;
}

QString RssItem::creator() const {
    return creator_;
}
