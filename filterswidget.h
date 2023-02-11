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
