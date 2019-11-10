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


#ifndef DIALOG_H
#define DIALOG_H

#include "pkgnamecompleter.h"

#include <QDialog>
#include <QDate>

namespace Ui {
class Dialog;
}

class QSqlTableModel;

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void applyFilters();
    void showAboutDialog();
    void selectCustomLogFile();
    void loadDefaultLog();

private:
    Ui::Dialog *ui;
    QSqlTableModel *model;
    PkgNameCompleter *pkgNameCompleter;
    QAction *loadCustomLog;
    QAction *openAboutDialog;
    QAction *reloadDefaultLog;
    QString lastPath;

    QDate oldestDate;

    void loadSettings();
    void readPacmanLogFile(const QString &logFile);
};

#endif // DIALOG_H
