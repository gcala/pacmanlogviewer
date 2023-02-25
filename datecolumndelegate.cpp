/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "datecolumndelegate.h"

#include <QPainter>
#include <QDate>

DateColumnDelegate::DateColumnDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QSize DateColumnDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return option.rect.size();
}

void DateColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDate date(index.data().toDate());

    int leftMargin = 4;

    QRect textRect(option.rect.x() + leftMargin,
                   option.rect.y(),
                   option.rect.width() - leftMargin,
                   option.rect.height());

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setPen(option.palette.highlightedText().color());
    } else {
        painter->setPen(option.palette.text().color());
    }

    QLocale::setDefault(QLocale::system());

    painter->drawText(textRect, Qt::AlignVCenter, QLocale::system().toString(date, QLocale::LongFormat));
}
