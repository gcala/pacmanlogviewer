/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "pkgnameslineedit.h"

PkgNamesLineEdit::PkgNamesLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this,SIGNAL(textChanged(QString)), this, SLOT(text_changed(QString)));
}

void PkgNamesLineEdit::text_changed(QString text)
{
    QString all_text = text;
    text = all_text.left(this->cursorPosition());
    QString prefix = text.split(",").last().trimmed();

    QStringList text_tags;
    foreach(QString s, all_text.split(",")) {
        QString t1 = s.trimmed();
        if(t1 != "")
            text_tags.append(s);
    }

    emit text_changed(text_tags, prefix);
}

void PkgNamesLineEdit::complete_text(QString text)
{
    int cursor_pos = this->cursorPosition();
    QString before_text = this->text().left(cursor_pos);
    QString after_text = this->text().right(this->text().length()-cursor_pos);
    int prefix_len = before_text.split(",").last().trimmed().length();
    this->setText(QString("%1%2, %3").arg(before_text.left(cursor_pos-prefix_len)).arg(text).arg(after_text));
    this->setCursorPosition(cursor_pos - prefix_len + text.length() + 2);
}
