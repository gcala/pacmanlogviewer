/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef ACTIONCOLUMNDELEGATE_H
#define ACTIONCOLUMNDELEGATE_H

#include <QItemDelegate>

class ActionColumnDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ActionColumnDelegate(QObject *parent = 0);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

#endif // ACTIONCOLUMNDELEGATE_H
