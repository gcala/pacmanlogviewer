/*
    Pacman Log Viewer
    Copyright (C) 2012-2019 Giuseppe Calà <jiveaxe@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

    QLocale locale;
    QLocale::setDefault(locale.name());

    painter->drawText(textRect, Qt::AlignVCenter, locale.toString(date, QLocale::LongFormat));
}
