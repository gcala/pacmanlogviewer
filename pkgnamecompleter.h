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
