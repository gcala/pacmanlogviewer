/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef DATECOLUMNDELEGATE_H
#define DATECOLUMNDELEGATE_H

#include <QItemDelegate>

class DateColumnDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DateColumnDelegate(QObject *parent = 0);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;    
};

#endif // DATECOLUMNDELEGATE_H
