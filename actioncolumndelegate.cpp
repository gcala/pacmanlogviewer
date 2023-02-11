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


#include "actioncolumndelegate.h"

#include <QPainter>

ActionColumnDelegate::ActionColumnDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QSize ActionColumnDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return option.rect.size();
}

void ActionColumnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString action = index.data().toString();
    QString icon;

    QSize iconSize(22,22);

    QRect iconRect(option.rect.x() + (option.rect.width()/2) - (iconSize.width()/2),
                   option.rect.y() + (option.rect.height()/2) - (iconSize.height()/2),
                   iconSize.width(),
                   iconSize.height());

    if(action == "installed")
        icon = "list-add";
    else if(action == "upgraded")
        icon = "go-up";
    else if(action == "removed")
        icon = "list-remove";
    else if(action == "downgraded")
        icon = "go-down";
    else if(action == "reinstalled")
        icon = "view-refresh";

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
        painter->setPen(option.palette.highlightedText().color());
    } else {
        painter->setPen(option.palette.text().color());
    }

    painter->drawPixmap(iconRect, QIcon::fromTheme(icon).pixmap(iconSize));
}
