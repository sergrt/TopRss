// RssParser class tests

#include "gtest/gtest.h"

#include "RssParser.h"

constexpr auto title = "Title";
constexpr auto descr = "Description";
constexpr auto link = "Link";
constexpr auto creator = "Creator";
constexpr auto author = "Author";

constexpr auto channelTitle = "Channel title";

namespace {
void logFunc(const QString& message) {
    std::cout << "Logged message: " << message.toStdString() << "\n";
}
}  // namespace

////////////////
// RssItem construction tests

TEST(RssParser, Construct_Item_From_Valid_Item) {
    const QString document = R"(<item>
        <title>)" + QString(title) + R"(</title>
        <description>)" + descr + R"(</description>
        <link>)" + link + R"(</link>
        <dc:creator>)" + creator + R"(</dc:creator>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    const auto item = RssParser::makeItem(elem);
    EXPECT_EQ(item.title().toStdString(), title);
    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_EQ(item.link().toStdString(), link);
    EXPECT_EQ(item.creator().toStdString(), creator);
}

TEST(RssParser, Construct_Item_From_Valid_Item_Use_Author_Field) {
    const QString document = R"(<item>
        <title>)" + QString(title) + R"(</title>
        <description>)" + descr + R"(</description>
        <link>)" + link + R"(</link>
        <author>)" + author + R"(</author>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    const auto item = RssParser::makeItem(elem);
    EXPECT_EQ(item.title().toStdString(), title);
    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_EQ(item.link().toStdString(), link);
    EXPECT_EQ(item.creator().toStdString(), author);
}

TEST(RssParser, Construct_Item_From_Valid_Item_Prefer_Creator) {
    const QString document = R"(<item>
        <title>)" + QString(title) + R"(</title>
        <description>)" + descr + R"(</description>
        <link>)" + link + R"(</link>
        <author>)" + author + R"(</author>
        <dc:creator>)" + creator + R"(</dc:creator>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    const auto item = RssParser::makeItem(elem);
    EXPECT_EQ(item.title().toStdString(), title);
    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_EQ(item.link().toStdString(), link);
    EXPECT_EQ(item.creator().toStdString(), creator);
}

TEST(RssParser, Construct_Item_From_Valid_Item_One_Mandatory_Title) {
    const QString document = R"(<item>
        <title>)" + QString(title) + R"(</title>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    const auto item = RssParser::makeItem(elem);
    EXPECT_EQ(item.title().toStdString(), title);
    EXPECT_TRUE(item.description().isEmpty());
    EXPECT_TRUE(item.link().isEmpty());
    EXPECT_TRUE(item.creator().isEmpty());
}

TEST(RssParser, Construct_Item_From_Valid_One_Mandatory_Descr) {
    const QString document = R"(<item>
        <description>)" + QString(descr) + R"(</description>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    const auto item = RssParser::makeItem(elem);
    EXPECT_TRUE(item.title().isEmpty());
    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_TRUE(item.link().isEmpty());
    EXPECT_TRUE(item.creator().isEmpty());
}

TEST(RssParser, Construct_Item_Form_Valid_With_Unhandled_Field) {
    const QString document = R"(<item>
        <description>)" + QString(descr) + R"(</description>
        <someUnhandledField>someUnhandledFieldValue</someUnhandledField>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    const auto item = RssParser::makeItem(elem);
    EXPECT_TRUE(item.title().isEmpty());
    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_TRUE(item.creator().isEmpty());
    EXPECT_TRUE(item.link().isEmpty());
}

TEST(RssParser, Construct_Item_From_Empty_Data) {
    const QString document = "";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    EXPECT_THROW(RssParser::makeItem(elem), std::runtime_error);
}

TEST(RssParser, Construct_Item_From_Empty_Item) {
    const QString document = R"(<item></item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    EXPECT_THROW(RssParser::makeItem(elem), std::runtime_error);
}

TEST(RssParser, Construct_Item_From_Item_Without_Mandatory) {
    const QString document = R"(<item>
        <link>)" + QString(link) + R"(</link>
        <dc:creator>)" + creator + R"(</dc:creator>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    EXPECT_THROW(RssParser::makeItem(elem), std::runtime_error);
}

TEST(RssParser, Construct_Item_From_Broken_Data) {
    const QString document = R"(<item>
        <titasf<dasf<dasf<</title>
    </item>)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    EXPECT_THROW(RssParser::makeItem(elem), std::runtime_error);
}

TEST(RssParser, Construct_Item_From_Garbage) {
    const QString document = R"(some<garbage<> data<here)";

    QDomDocument doc;
    doc.setContent(document);
    const QDomElement node = doc.documentElement();
    const QDomElement elem = node.toElement();

    EXPECT_THROW(RssParser::makeItem(elem), std::runtime_error);
}

////////////////
// Document parsing tests

TEST(RssParser, Parse_Valid_Document_1_Item) {
    const QString document = R"(<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet title="XSL_formatting" type="text/xsl" href="/shared/bsp/xsl/rss/nolsol.xsl"?>
<rss>
    <channel>
        <title>Channel title</title>
        <link>Channel link</link>
        <description>Channel description</description>
        <item>
            <title>)" + QString(title) + R"(</title>
            <description>)" + descr + R"(</description>
            <link>)" + link + R"(</link>
            <dc:creator>)" + creator + R"(</dc:creator>
        </item>
    </channel>
</rss>)";

    const auto items = RssParser::extractItems(document, logFunc);
    ASSERT_EQ(items.size(), 1);
    const auto& item = items[0];
    EXPECT_EQ(item.title().toStdString(), title);
    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_EQ(item.link().toStdString(), link);
    EXPECT_EQ(item.creator().toStdString(), creator);
}

TEST(RssParser, Parse_Valid_Document_0_Items) {
    const QString document = R"(<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet title="XSL_formatting" type="text/xsl" href="/shared/bsp/xsl/rss/nolsol.xsl"?>
<rss>
    <channel>
        <title>Channel title</title>
        <link>Channel link</link>
        <description>Channel description</description>
    </channel>
</rss>)";

    const auto items = RssParser::extractItems(document, logFunc);
    EXPECT_EQ(items.size(), 0);
}

TEST(RssParser, Parse_Valid_Document_2_Items) {
    const QString document = R"(<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet title="XSL_formatting" type="text/xsl" href="/shared/bsp/xsl/rss/nolsol.xsl"?>
<rss>
    <channel>
        <title>Channel title</title>
        <link>Channel link</link>
        <description>Channel description</description>
        <item>
            <title>)" + QString(title) + "0" + R"(</title>
            <description>)" + descr + "0" + R"(</description>
            <link>)" + link + "0" + R"(</link>
            <dc:creator>)" + creator + "0" + R"(</dc:creator>
        </item>
        <item>
            <title>)" + QString(title) + "1" + R"(</title>
            <description>)" + descr + "1" + R"(</description>
            <link>)" + link + "1" + R"(</link>
            <dc:creator>)" + creator + "1" + R"(</dc:creator>
        </item>
    </channel>
</rss>)";

    const auto items = RssParser::extractItems(document, logFunc);
    ASSERT_EQ(items.size(), 2);
    for (size_t i = 0; i < 2; ++i) {
        const auto& item = items[i];
        EXPECT_EQ(item.title().toStdString(), title + QString("%1").arg(i).toStdString());
        EXPECT_EQ(item.description().toStdString(), descr + QString("%1").arg(i).toStdString());
        EXPECT_EQ(item.link().toStdString(), link + QString("%1").arg(i).toStdString());
        EXPECT_EQ(item.creator().toStdString(), creator + QString("%1").arg(i).toStdString());
    }
}

TEST(RssParser, Parse_Invalid_Document) {
    const QString document = R"(<some<garbage data>@#$^ here)";

    const auto items = RssParser::extractItems(document, logFunc);
    EXPECT_EQ(items.size(), 0);
}

TEST(RssParser, Parse_Empty_Document) {
    const QString document = "";

    const auto items = RssParser::extractItems(document, logFunc);
    EXPECT_EQ(items.size(), 0);
}

////////////////
// Channel title tests

TEST(RssParser, Channel_Title_From_Valid_Document) {
    const QString document = R"(<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet title="XSL_formatting" type="text/xsl" href="/shared/bsp/xsl/rss/nolsol.xsl"?>
<rss>
    <channel>
        <title>)" + QString(channelTitle) + R"(</title>
        <link>Channel link</link>
        <description>Channel description</description>
        <item>
            <title>Item title</title>
        </item>
    </channel>
</rss>)";

    const auto t = RssParser::channelTitle(document, logFunc);
    EXPECT_EQ(t.toStdString(), channelTitle);
}

TEST(RssParser, Channel_Title_From_Mailformed_Document) {
    const QString document = R"(<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet title="XSL_formatting" type="text/xsl" href="/shared/bsp/xsl/rss/nolsol.xsl"?>
<rss>
    <channel>
        <item>
            <title>Item title</title>
        </item>
    </channel>
</rss>)";

    QString out;
    const auto t = RssParser::channelTitle(document, [this, &out](const QString& message) {
        logFunc(message);
        out = message;
    });
    EXPECT_TRUE(t.isEmpty());
    EXPECT_FALSE(out.isEmpty());
}

TEST(RssParser, Channel_Title_From_Document_With_Unhandled_Fields) {
    const QString document = R"(<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet title="XSL_formatting" type="text/xsl" href="/shared/bsp/xsl/rss/nolsol.xsl"?>
<rss>
    <channel>
        <someUnhandledField>someUnhandledFieldValue</someUnhandledField>
        <title>)" + QString(channelTitle) + R"(</title>
        <link>Channel link</link>
        <description>Channel description</description>
        <item>
            <title>Item title</title>
        </item>
    </channel>
</rss>)";

    const auto t = RssParser::channelTitle(document, logFunc);
    EXPECT_EQ(t.toStdString(), channelTitle);
}

TEST(RssParser, Channel_Title_From_Empty_Document) {
    const QString document = "";

    QString out;
    const auto t = RssParser::channelTitle(document, [this, &out](const QString& message) {
        logFunc(message);
        out = message;
    });
    EXPECT_TRUE(t.isEmpty());
    EXPECT_FALSE(out.isEmpty());
}

TEST(RssParser, Channel_Title_From_Garbage) {
    const QString document = "garbage< data< here>";

    QString out;
    const auto t = RssParser::channelTitle(document, [this, &out](const QString& message) {
        logFunc(message);
        out = message;
    });
    EXPECT_TRUE(t.isEmpty());
    EXPECT_FALSE(out.isEmpty());
}
