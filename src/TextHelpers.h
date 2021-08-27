#pragma once
#include <QLabel>
#include <QString>

// Return elided text with ellipsis, that fits into supplied label
inline QString elidedText(const QLabel& label, const QString& text) {
    return label.fontMetrics().elidedText(text, Qt::ElideRight, label.contentsRect().width());
}