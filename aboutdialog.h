/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
    
};

#endif // ABOUTDIALOG_H
