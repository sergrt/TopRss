// RssItem class tests

#include "gtest/gtest.h"

#include "RssItem.h"

constexpr auto creator = "Creator";
constexpr auto title = "Title";
constexpr auto descr = "Description";
constexpr auto link = "Link";

TEST(RssItem, Construct_Item_With_Valid_Fields) {
    RssItem::RssItemBuilder item_builder;
    const auto item = item_builder.with_creator(creator)
                                  .with_title(title)
                                  .with_description(descr)
                                  .build();

    EXPECT_EQ(item.creator().toStdString(), creator);
    EXPECT_EQ(item.title().toStdString(), title);
    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_TRUE(item.link().isEmpty());
}

TEST(RssItem, Construct_Item_With_One_Mandatory_Title) {
    RssItem::RssItemBuilder item_builder;
    const auto item = item_builder.with_title(title).build();

    EXPECT_EQ(item.title().toStdString(), title);
    EXPECT_TRUE(item.creator().isEmpty());
    EXPECT_TRUE(item.description().isEmpty());
    EXPECT_TRUE(item.link().isEmpty());
}

TEST(RssItem, Construct_Item_With_One_Mandatory_Descr) {
    RssItem::RssItemBuilder item_builder;
    const auto item = item_builder.with_description(descr).build();

    EXPECT_EQ(item.description().toStdString(), descr);
    EXPECT_TRUE(item.creator().isEmpty());
    EXPECT_TRUE(item.title().isEmpty());
    EXPECT_TRUE(item.link().isEmpty());
}

TEST(RssItem, Construct_Item_Witn_No_Data) {
    RssItem::RssItemBuilder item_builder;
    EXPECT_THROW(item_builder.build(), std::runtime_error);
}

TEST(RssItem, Construct_Item_Without_Mandatory) {
    RssItem::RssItemBuilder item_builder;
    EXPECT_THROW(item_builder.with_creator(creator)
                             .with_link(link)
                             .build(),
                 std::runtime_error);
}
