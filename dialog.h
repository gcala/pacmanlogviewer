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
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    
private slots:
    void applyFilters();
    void selectCustomLogFile();

private:
    Ui::Dialog *ui;
    QSqlTableModel *m_model;
    PkgNameCompleter *m_pkgNameCompleter;
    QAction *m_customLogAction;
    QAction *m_aboutAction;
    QAction *m_reloadAction;
    QString m_lastPath;
    QDate m_oldestDate;

    void loadSettings();
    void readPacmanLogFile(const QString &logFile);
    void setupTranslations();
};

#endif // DIALOG_H
