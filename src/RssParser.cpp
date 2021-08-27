#include "RssParser.h"

#include <QtXml>

namespace TagName {
// Top level tags
constexpr auto channel{"channel"};
constexpr auto item{"item"};

// channel tags
constexpr auto channelTitle{"title"};

// item tags
constexpr auto title{"title"};
constexpr auto description{"description"};
constexpr auto pubDate{"pubDate"};
constexpr auto creator{"dc:creator"};
constexpr auto author{"author"};
constexpr auto link{"link"};
}  // namespace TagName

std::vector<RssItem> RssParser::extractItems(const QString& document,
                                             const std::function<void(const QString&)>& logFunc) {
    QDomDocument doc;
    doc.setContent(document);
    const QDomElement docElem = doc.documentElement();

    QDomNode node = docElem.firstChild();
    const QDomElement nodeElement = node.toElement();
    if (nodeElement.isNull() || nodeElement.tagName() != TagName::channel) {
        logFunc(QObject::tr("Cannot find channel node: invalid RSS document"));
        return {};
    }
    node = nodeElement.firstChild();

    std::vector<RssItem> res;
    while (!node.isNull()) {
        if (QDomElement e = node.toElement(); !e.isNull() && e.tagName() == TagName::item) {
            try {
                res.emplace_back(makeItem(e));
            } catch (std::exception& exception) {
                logFunc(exception.what());
            }
        }
        node = node.nextSibling();
    }
    return res;
}

RssItem RssParser::makeItem(const QDomElement& element) {
    auto childNode = element.firstChild();
    RssItem::RssItemBuilder itemBuilder;
    QString creator;
    while (!childNode.isNull()) {
        if (const auto elem = childNode.toElement(); !elem.isNull()) {
            const auto tag_name = elem.tagName();
            const auto text = elem.text();
            if (tag_name == TagName::description) {
                itemBuilder.with_description(text);
            } else if (tag_name == TagName::title) {
                itemBuilder.with_title(text);
            } else if (tag_name == TagName::link) {
                itemBuilder.with_link(text);
            } else if (tag_name == TagName::pubDate) {
                itemBuilder.with_pubDate(QDateTime::fromString(text, Qt::RFC2822Date));
            } else if (tag_name == TagName::author) {
                if (creator.isEmpty())  // Prefer dc:creator over author, so remember value in case dc:creator is missing
                    creator = text;
            } else if (tag_name == TagName::creator) {
                creator = text;
            }
        }
        childNode = childNode.nextSibling();
    }
    return itemBuilder.with_creator(creator).build();
}

QString RssParser::channelTitle(const QString& document, const std::function<void(const QString&)>& logFunc) {
    QDomDocument doc;
    doc.setContent(document);
    const QDomElement docElem = doc.documentElement();

    QDomNode node = docElem.firstChild();
    const QDomElement nodeElement = node.toElement();
    if (nodeElement.isNull() || nodeElement.tagName() != TagName::channel) {
        logFunc(QObject::tr("Cannot obtain channel title: invalid RSS document"));
        return {};
    }
    node = nodeElement.firstChild();

    while (!node.isNull()) {
        if (QDomElement e = node.toElement(); !e.isNull() && e.tagName() == TagName::channelTitle) {
            return e.text();
        }
        node = node.nextSibling();
    }

    // channel title field is mandatory, but if it's missing it has only little impact on UX, so we just log this and
    // consider this as non-fatal event
    logFunc(QObject::tr("Could not obtain channel title"));
    return {};
}
