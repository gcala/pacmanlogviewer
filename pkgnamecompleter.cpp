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


#include "pkgnamecompleter.h"

PkgNameCompleter::PkgNameCompleter(QObject *parent) :
    QCompleter(parent)
{
    model = new QStringListModel;
}

void PkgNameCompleter::update(QStringList text_tags, QString completion_prefix)
{
    QStringList tags(all_tags);
    foreach(QString s, text_tags) {
        if(text_tags.indexOf(s) == (text_tags.count() - 1) )
            tags.removeAll(s.trimmed().left(s.size()-1));
        else
            tags.removeAll(s.trimmed());
    }

    model->setStringList(tags);

    this->setModel(model);

    this->setCompletionPrefix(completion_prefix);
    if(completion_prefix.trimmed() != "")
        this->complete();
}

void PkgNameCompleter::setTags(const QStringList &tags)
{
    all_tags = tags;
}
