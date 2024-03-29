/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
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
    ui->fromDateEdit->setLocale(QLocale::system());
    ui->toDateEdit->setLocale(QLocale::system());

    const auto checkBoxes = findChildren< QCheckBox * >();
    for(auto cb : checkBoxes) {
        connect(cb, &QCheckBox::toggled, this, &FiltersWidget::filtersChanged);
    }
    const auto dateEdits = findChildren< QDateEdit * >();
    for(auto de : dateEdits) {
        connect(de, &QDateEdit::dateChanged, this, &FiltersWidget::filtersChanged);
    }

    connect(ui->dateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=] (int index) {
        ui->fromDateEdit->setEnabled(index == 5 ? true : false);
        ui->toDateEdit->setEnabled(index == 5 ? true : false);

        emit filtersChanged();
    });

    connect(ui->toggleFiltersButton, &QToolButton::toggled, this, [=](bool shown) {
        ui->FiltersContainer->setHidden(!shown);
        ui->toggleFiltersButton->setArrowType(shown ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
    });
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

bool FiltersWidget::downgradedChecked()
{
    return ui->downgradedCheckBox->isChecked();
}

void FiltersWidget::setDowngradedCB(const bool &checked)
{
    ui->downgradedCheckBox->setChecked(checked);
}

bool FiltersWidget::reinstalledChecked()
{
    return ui->reinstalledCheckBox->isChecked();
}

void FiltersWidget::setReinstalledCB(const bool &checked)
{
    ui->reinstalledCheckBox->setChecked(checked);
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
