#include "UrlFetcher.h"

#include <QNetworkReply>
#include <QRegularExpression>

void UrlFetcher::fetch(const QUrl& url) {
    QNetworkReply* response = manager_.get(QNetworkRequest(url));

    connect(response, &QNetworkReply::finished, [this, response] {
        response->deleteLater();

        if (response->error() != QNetworkReply::NoError) {
            emit fetchError(tr("Network error : ") + response->errorString());
            return;
        }

        const auto contentType = response->header(QNetworkRequest::ContentTypeHeader).toString();
        if (!checkEncodingIsUtf8(contentType)) {
            emit fetchError(tr("Content charsets other than utf-8 are not supported: ") + contentType);
            return;
        }

        auto const html = QString::fromUtf8(response->readAll());
        emit dataReady(response->url(), html);
    });
}

bool UrlFetcher::checkEncodingIsUtf8(const QString& contentType) {
    static QRegularExpression re("charset=([!-~]+)");
    auto const match = re.match(contentType);
    return match.hasMatch() && match.captured(1).compare("utf-8", Qt::CaseInsensitive) == 0;
}
