#pragma once
// Main UI window class

#include "ui_TopRss.h"

#include "ItemListModel.h"
#include "RssItemsFetcher.h"

#include <QtWidgets/QMainWindow>
#include <QUrl>

class TopRss final : public QMainWindow {
    Q_OBJECT

public:
    TopRss(QWidget* parent = nullptr);

private:
    Ui::TopRssClass ui_;
    ItemListModel model_;
    RssItemsFetcher fetcher_;

private slots:
    void fetchSelectedSource();
    void addSource();
    void addLogRecord(const QString& record);
    void processRssItems(const QString& channelTitle, const std::vector<RssItem>& items);
    void switchViewMode(bool compact);
};
