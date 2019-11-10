/*
    Pacman Log Viewer
    Copyright (C) 2012-2015 Giuseppe Calà <jiveaxe@gmail.com>

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
