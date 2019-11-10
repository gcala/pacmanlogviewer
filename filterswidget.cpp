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


#include "filterswidget.h"
#include "ui_filterswidget.h"

FiltersWidget::FiltersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FiltersWidget)
{
    ui->setupUi(this);
    ui->FiltersContainer->setHidden(true);

    ui->fromDateEdit->setEnabled(false);
    ui->toDateEdit->setEnabled(false);
    ui->fromDateEdit->setLocale(QLocale::system().name());
    ui->toDateEdit->setLocale(QLocale::system().name());
    
    // Qt5 uses wrong icon size
    ui->toggleFiltersButton->setIconSize(QSize(16,16));

    if(QIcon::hasThemeIcon(QLatin1String("draw-triangle2")))
        ui->toggleFiltersButton->setIcon(QIcon::fromTheme(QLatin1String("draw-triangle2")));
    else
        ui->toggleFiltersButton->setIcon(QIcon::fromTheme(QLatin1String("go-down"), QIcon(QLatin1String(":/images/draw-triangle2.png"))));

    connect(ui->installedCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(filtersChanged()));
    connect(ui->upgradedCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(filtersChanged()));
    connect(ui->removedCheckBox, SIGNAL(toggled(bool)), this, SIGNAL(filtersChanged()));
    connect(ui->fromDateEdit, SIGNAL(dateChanged(QDate)), this, SIGNAL(filtersChanged()));
    connect(ui->toDateEdit, SIGNAL(dateChanged(QDate)), this, SIGNAL(filtersChanged()));
    connect(ui->dateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dateRangeChanged(int)));
}

FiltersWidget::~FiltersWidget()
{
    delete ui;
}

bool FiltersWidget::installedChecked()
{
    return ui->installedCheckBox->isChecked();
}

void FiltersWidget::setInstallCB(const bool &checked)
{
    ui->installedCheckBox->setChecked(checked);
}

bool FiltersWidget::upgradedChecked()
{
    return ui->upgradedCheckBox->isChecked();
}

void FiltersWidget::setUpdatedCB(const bool &checked)
{
    ui->upgradedCheckBox->setChecked(checked);
}

bool FiltersWidget::removedChecked()
{
    return ui->removedCheckBox->isChecked();
}

void FiltersWidget::setRemovedCB(const bool &checked)
{
    ui->removedCheckBox->setChecked(checked);
}

int FiltersWidget::dateRangeIndex()
{
    return ui->dateComboBox->currentIndex();
}

void FiltersWidget::setRangeIndex(const int &index)
{
    ui->dateComboBox->setCurrentIndex(index);
}

QDate FiltersWidget::fromDate()
{
    return ui->fromDateEdit->date();
}

void FiltersWidget::setFromDate(const QDate &date)
{
    ui->fromDateEdit->setDate(date);
}

QDate FiltersWidget::toDate()
{
    return ui->toDateEdit->date();
}

void FiltersWidget::setToDate(const QDate &date)
{
    ui->toDateEdit->setDate(date);
}

void FiltersWidget::on_toggleFiltersButton_clicked()
{
    ui->FiltersContainer->setHidden(!ui->FiltersContainer->isHidden());

    if(ui->FiltersContainer->isHidden()) {
        if(QIcon::hasThemeIcon(QLatin1String("draw-triangle2")))
        ui->toggleFiltersButton->setIcon(QIcon::fromTheme(QLatin1String("draw-triangle2")));
    else
        ui->toggleFiltersButton->setIcon(QIcon::fromTheme(QLatin1String("go-down"), QIcon(QLatin1String(":/images/draw-triangle2.png"))));
    } else {
        if(QIcon::hasThemeIcon(QLatin1String("draw-triangle4")))
        ui->toggleFiltersButton->setIcon(QIcon::fromTheme(QLatin1String("draw-triangle4")));
    else
        ui->toggleFiltersButton->setIcon(QIcon::fromTheme(QLatin1String("go-up"), QIcon(QLatin1String(":/images/draw-triangle4.png"))));
    }
}

void FiltersWidget::dateRangeChanged(int index)
{
    ui->fromDateEdit->setEnabled(index == 5 ? true : false);
    ui->toDateEdit->setEnabled(index == 5 ? true : false);

    emit filtersChanged();
}
