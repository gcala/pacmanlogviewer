/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <QApplication>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog w;
    w.show();
    
    return a.exec();
}
