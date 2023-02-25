/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
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
