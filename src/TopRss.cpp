#include "TopRss.h"

#include "ItemDelegate.h"

#include <QFontDatabase>

constexpr auto predefinedFeeds = {
    "http://feeds.bbci.co.uk/news/rss.xml?edition=uk",
    "https://habr.com/ru/rss/all/all/?fl=ru",
    "https://herbsutter.com/feed/",
    "http://www.nuonsoft.com/blog/feed/",
    "https://www.ixbt.com/export/news.rss",
    "https://www.ferra.ru/exports/rss.xml",
    "https://3dnews.ru/breaking/rss/",
};
constexpr int articlesSpacing = 1;

TopRss::TopRss(QWidget* parent) : QMainWindow(parent), fetcher_(this) {
    ui_.setupUi(this);

    ui_.listView->setItemDelegate(new ItemDelegate{ui_.listView, ui_.compactView->isChecked()});
    ui_.listView->setModel(&model_);
    ui_.listView->setAttribute(Qt::WA_Hover);
    ui_.listView->setSpacing(articlesSpacing);

    for (const auto& feed : predefinedFeeds) {
        ui_.sourcesList->addItem(feed);
    }

    ui_.logView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    connect(&fetcher_, &RssItemsFetcher::itemsReady, this, &TopRss::processRssItems);
    connect(&fetcher_, &RssItemsFetcher::logRecord, this, &TopRss::addLogRecord);

    connect(ui_.fetchButton, &QPushButton::clicked, this, &TopRss::fetchSelectedSource);
    connect(ui_.addSource, &QPushButton::clicked, this, &TopRss::addSource);
    connect(ui_.compactView, &QCheckBox::stateChanged, this, [this](int checked) { switchViewMode(checked != 0); });
}

void TopRss::addLogRecord(const QString& record) {
    const auto stamp = QDateTime::currentDateTimeUtc().toString("[hh:mm:ss]");
    ui_.logView->append(stamp + " " + record);
}

void TopRss::switchViewMode(bool compact) {
    auto curDelegate = ui_.listView->itemDelegate();
    ui_.listView->setItemDelegate(new ItemDelegate{ui_.listView, compact});
    curDelegate->deleteLater();
}

void TopRss::fetchSelectedSource() {
    model_.clearRssData();
    ui_.currentChannel->setText({});

    if (const auto item = ui_.sourcesList->currentItem(); item) {
        const auto source = item->text();
        addLogRecord(tr("Fetching from url: ") + source);
        fetcher_.fetch(QUrl(source));
    } else {
        addLogRecord(tr("Nothing selected to be fetched"));
    }
}

void TopRss::addSource() {
    ui_.sourcesList->addItem(ui_.url->text());
}

void TopRss::processRssItems(const QString& channelTitle, const std::vector<RssItem>& items) {
    addLogRecord(tr("Processing items from channel: \"%1\"").arg(channelTitle));
    model_.setRssData(items);
    ui_.currentChannel->setText(channelTitle);
}
