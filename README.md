## About
__TopRss__ is a simple RSS feed reader made as an assessment test. This repository contains source code along with precompiled binaries - downloads are available in [Releases](https://github.com/sergrt/TopRss/releases) section.

## Task description
- Small desktop application displaying a list of news of any given RSS news channel.
- Every item in the list must contain a link by clicking on which a default web browser opens it.
- GUI design itself does not matter that much, the major point is that it should be straightforward and simple.
- The GUI must be written with plain Qt Widgets. The logic itself is on C++/Qt5.
- User must be able to put any valid RSS channel (e.g. http://feeds.bbci.co.uk/news/rss.xml?edition=uk) or be prompted accordingly if the URL is not valid.
- Unit-tests are greatly encouraged.

## Overview
### General notes
TopRss application is based on Qt widgets, and uses no extra libraries (except googletest for unit-testing). Despite it's a quite simple application, there was an attempt made to provide user with modern UI experience like dynamic text reflow and nice eliding of long lines with ellipsis.

Application UI has following noticable parts:
- Predefined RSS feeds list
- RSS articles view
- Compact/regular RSS view mode switch
- Current channel title
- Log/error output view
- Input field with button to add user-defined RSS feed URL to feeds list.

Application session can be like this (compact view is off):
![Screenshot](/docs/images/TopRss_window.png)

Feed is fetched by being selected from the list of URLs and pressing "Fetch selected" button. Operations regarding current network commutications and parsing operations are displayed in the log view at the bottom of the screen. Error messages are displayed in the same log view as well.
Each article in regular view mode contains following information (top to bottom, left to right):
1. Title
2. Publication date
3. Article creator (author)
4. Description
5. Clickable URL

Compact view shows articles in a very condensed form, displaying only clickable title:
![Screenshot](/docs/images/TopRss_window_compact.png)

Click on `Read more: <URL>` in regular view mode (or title of the article in compact view) opens article in a default browser.

Above the top of the articles list there's a title of a current channel.

## Implementation details
### General architecture
Despite application is really "small" as stated in Task Description, there decision was taken to implement architechtural layers - backend, business logics and UI. This approach enables application to be extensible, easily comprehensible, less error-prone on modifications, and last but not least allows business logics to be tested with unit-tests. But there were no overengineering performed - for the sake of simplicity dependency inversion (BE-BL-UI) was not considered as an option.

List of classes by layer looks as following:

- Backend classes:
  - `UrlFetcher` - fetching document from specified URL
- BL classes:
   - `RssItem` - RSS Item - article
   - `RssItemsFetcher` - articles fetcher
   - `RssParser` - raw document parser
- UI classes:
  - `TopRss` - main class
  - `ItemListModel` - feed model
  - `ItemDelegate` - delegate for article list view
  - `ItemRenderer` - interface for item renderer
  - `ItemRenderWidget` - widget for rendering article item (regular)
  - `ItemRenderCompactWidget` - widget for rendering article item (compact)

### UI implementation details
Main UI class uses `RssItemsFetcher` to obtain items and channel title. `RssItemsFetcher` on it's turn uses backend to get items from desired source. Currently only URLs implemented, but this can be easily extended to SQL database (in case items are fetched in background) or something else.
Articles list is implemented as `QListView` with simple model. List items is displayed with `ItemDelegate`. Delegate __uses regular widgets for painting__ - this provides us with luxury of using Qt Designer, designers' blueprints and other regular workflow without need of manually creating/sizing/placing widgets.
`ItemDelegate` manipulates with regular and compact rendering widgets via `ItemRenderer` interface, so it should be quite easy to extend view modes.

### RSS specification compliance
Some of the aspects of RSS 2.0 specifications were taken into consideration while developing TopRss:
- XML document should be valid (how else)
- Missing channel title considered as non-fatal error and is reported
- Missing mandatory item fields (at least one of title or description must be present) considered as error, item construction throws and exception, and error is reported
- Article creator is obtained from `dc:creator` node, or, if it's missing, from `author` node.

### Unit-tests overview
Business logics is tested with some unit-tests. Tests are implemented using googletest library. Handy "UnityBuild" pattern was used to provide test project with needed files.

## Example of how to build
Just for sake of recalling there are quick steps for building application (Windows, Visual Studio 2019):
1. Clone repository
2. Open src/CMakeLists.txt and set CMAKE_PREFIX_PATH pointing to Qt installation
3. Make dir inside cloned directory named "build"
4. From "build" directory (step 3) call:
   `$ cmake .. -G "Visual Studio 16 2019" .`
5. To make unit tests work clone googletest into "test/gtest"
   `git clone https://github.com/google/googletest.git gtest`