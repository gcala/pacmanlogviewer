/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

// version include
#include <config-version.h>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->versionLabel->setText(PLV_VERSION);

    connect(ui->closeButton, &QPushButton::clicked, this, [=]() {
        this->close();
    });
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
