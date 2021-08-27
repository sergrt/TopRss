#pragma once
//Backend class to retrieve document from URL

#include <QNetworkAccessManager>

class UrlFetcher final : public QObject {
    Q_OBJECT

public:
    void fetch(const QUrl& url);

private:
    static bool checkEncodingIsUtf8(const QString& contentType);

    QNetworkAccessManager manager_;

signals:
    void dataReady(const QUrl& url, const QString& document);
    void fetchError(const QString& message);
};
