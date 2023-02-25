/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "pkgnameslineedit.h"

PkgNamesLineEdit::PkgNamesLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(text_changed(QString)));
}

void PkgNamesLineEdit::text_changed(QString text)
{
    const auto all_text = text;
    text = all_text.left(this->cursorPosition());
    const auto prefix = text.split(",").last().trimmed();

    QStringList text_tags;
    for(const auto &s : all_text.split(",")) {
        const auto tag = s.trimmed();
        if(tag != "")
            text_tags.append(tag);
    }

    emit text_changed(text_tags, prefix);
}

void PkgNamesLineEdit::complete_text(QString text)
{
    const int cursor_pos = cursorPosition();
    const auto before_text = this->text().left(cursor_pos);
    const auto after_text = this->text().right(this->text().length() - cursor_pos);
    const auto prefix_len = before_text.split(",").last().trimmed().length();
    setText(QString("%1%2, %3").arg(before_text.left(cursor_pos - prefix_len), text, after_text));
    setCursorPosition(cursor_pos - prefix_len + text.length() + 2);
}
