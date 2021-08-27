#include "RssItemsFetcher.h"

#include "RssParser.h"

RssItemsFetcher::RssItemsFetcher(QObject* parent) : QObject(parent) {
    connect(&fetcher_, &UrlFetcher::fetchError, this, [this](const QString& message) { emit logRecord(message); });
    connect(&fetcher_, &UrlFetcher::dataReady, this, &RssItemsFetcher::processRssData);
}

void RssItemsFetcher::fetch(const QUrl& source) {
    fetcher_.fetch(source);
}

void RssItemsFetcher::processRssData(const QUrl& url, const QString& document) {
    auto logFunc = [this](const QString& message) { emit logRecord(message); };
    const auto items = RssParser::extractItems(document, logFunc);
    const auto channelTitle = RssParser::channelTitle(document, logFunc);

    if (items.empty() && channelTitle.isEmpty()) {
        emit logRecord(tr("No valid data received from URL ") + url.toString());
    } else {
        emit itemsReady(channelTitle, items);
    }
}
