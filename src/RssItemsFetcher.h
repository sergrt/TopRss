#pragma once
// BL class to provide UI with items to display

#include "RssItem.h"
#include "UrlFetcher.h"

#include <QObject>

#include <vector>

class RssItemsFetcher final : public QObject {
    Q_OBJECT

public:
    RssItemsFetcher(QObject* parent);

    void fetch(const QUrl& source);

private:
    UrlFetcher fetcher_;

signals:
    void itemsReady(const QString& channelTitle, const std::vector<RssItem>& items);
    void logRecord(const QString& record);

private slots:
    void processRssData(const QUrl& url, const QString& document);
};
