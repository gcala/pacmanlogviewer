/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#ifndef FILTERSWIDGET_H
#define FILTERSWIDGET_H

#include <QWidget>
#include <QDate>

namespace Ui {
class FiltersWidget;
}

class FiltersWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit FiltersWidget(QWidget *parent = 0);
    ~FiltersWidget();

    bool installedChecked();
    void setInstallCB(const bool &);
    bool upgradedChecked();
    void setUpdatedCB(const bool &);
    bool removedChecked();
    void setRemovedCB(const bool &);
    bool downgradedChecked();
    void setDowngradedCB(const bool &);
    bool reinstalledChecked();
    void setReinstalledCB(const bool &);
    int dateRangeIndex();
    void setRangeIndex(const int &);
    QDate fromDate();
    void setFromDate(const QDate &);
    QDate toDate();
    void setToDate(const QDate &);
    
private slots:
    void filtersToggled(bool);
    void dateRangeChanged(int);

signals:
    void filtersChanged();

private:
    Ui::FiltersWidget *ui;
};

#endif // FILTERSWIDGET_H
