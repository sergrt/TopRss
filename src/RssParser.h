#pragma once
// This class is a parser of xml data, obtained from HTTP(S) backend.

#include "RssItem.h"

#include <QDomElement>

#include <vector>

class RssParser final {
public:
    static std::vector<RssItem> extractItems(const QString& document,
                                             const std::function<void(const QString&)>& logFunc);
    static QString channelTitle(const QString& document, const std::function<void(const QString&)>& logFunc);
    static RssItem makeItem(const QDomElement& element);
};
