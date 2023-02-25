/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef PKGNAMECOMPLETER_H
#define PKGNAMECOMPLETER_H

#include <QCompleter>
#include <QStringList>
#include <QStringListModel>

class PkgNameCompleter : public QCompleter
{
    Q_OBJECT
public:
    explicit PkgNameCompleter(QObject *parent = 0);
    void setTags(const QStringList &tags);
    
signals:
    
public slots:
    void update(QStringList, QString);

private:
    QStringList all_tags;
    QStringListModel* model;
};

#endif // PKGNAMECOMPLETER_H
