#pragma once
// This class represents Rss item record obtained from backend (HTTP(S) source, database etc.)

#include <QDateTime>
#include <QString>

class RssItem {
private:
    struct RssItemProperties {
        QString title;
        QString description;
        QString link;
        QDateTime pubDate;
        QString creator;
    };

public:
    // Use builder pattern to avoid huge constructors or exposing extra supplementary structures to users of the RssItem
    // class. Snake case w. camel case mixing here looks useful when reading the code.
    class RssItemBuilder {
    public:
        RssItemBuilder& with_title(const QString& title);
        RssItemBuilder& with_description(const QString& description);
        RssItemBuilder& with_link(const QString& link);
        RssItemBuilder& with_creator(const QString& creator);
        RssItemBuilder& with_pubDate(const QDateTime& pubDate);
        RssItem build() const;

    private:
        RssItemProperties properties_;
    };

    QString title() const;
    QString description() const;
    QString link() const;
    QString creator() const;
    QDateTime pubDate() const;

private:
    RssItem(const RssItemProperties& properties);

    QString title_;
    QString description_;
    QString link_;
    QDateTime pubDate_;
    QString creator_;
};
