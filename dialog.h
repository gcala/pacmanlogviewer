/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
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
    void setupTranslations();
};

#endif // DIALOG_H
