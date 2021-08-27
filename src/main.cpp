#include "TopRss.h"

#include <QtWidgets/QApplication>

constexpr auto stylesheet = R"(
    QCheckBox::indicator {
        width: 24px;
        height: 24px;
    }
    QCheckBox::indicator:checked {
        image: url(:/TopRss/res/switch_on.png);
    }
    QCheckBox::indicator:unchecked {
        image: url(:/TopRss/res/switch_off.png);
    }
)";

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyleSheet(stylesheet);
    TopRss w;
    w.show();
    return a.exec();
}
