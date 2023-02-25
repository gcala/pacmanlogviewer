/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef PKGNAMESLINEEDIT_H
#define PKGNAMESLINEEDIT_H

#include <QLineEdit>

class PkgNamesLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit PkgNamesLineEdit(QWidget *parent = 0);
    
signals:
    void text_changed(QStringList, QString);
    
public slots:
    void text_changed(QString);
    void complete_text(QString);
};

#endif // PKGNAMESLINEEDIT_H
