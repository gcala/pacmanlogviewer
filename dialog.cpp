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


#include "dialog.h"
#include "ui_dialog.h"
#include "aboutdialog.h"

#include "connection.h"
#include "datecolumndelegate.h"
#include "actioncolumndelegate.h"

#include <QSqlTableModel>
#include <QtWidgets>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->tableView->setSortingEnabled(true);

    QList<QPushButton *> buttonList = this->findChildren<QPushButton *>();
    foreach(QPushButton *pb, buttonList) {
        pb->setDefault( false );
        pb->setAutoDefault( false );
    }

    if(!createConnection())
        return;
    
    QSqlQuery query("CREATE TABLE log (id INTEGER PRIMARY KEY, date TEXT, op TEXT, pkg TEXT, ver TEXT)");
    query.exec();
    
    model = new QSqlTableModel;
    model->setTable("log");
    model->setHeaderData( 1, Qt::Horizontal, tr("Date") );
    model->setHeaderData( 2, Qt::Horizontal, tr("Action") );
    model->setHeaderData( 3, Qt::Horizontal, tr("Package") );
    model->setHeaderData( 4, Qt::Horizontal, tr("Version") );

    ui->tableView->setModel(model);
    
    pkgNameCompleter = new PkgNameCompleter(ui->namesLineEdit);
    pkgNameCompleter->setWidget(ui->namesLineEdit);
    connect(ui->namesLineEdit, SIGNAL(text_changed(QStringList,QString)), pkgNameCompleter, SLOT(update(QStringList,QString)));
    connect(pkgNameCompleter, SIGNAL(activated(QString)), ui->namesLineEdit, SLOT(complete_text(QString)));
    
    QMenu *menu = new QMenu(this);
    reloadDefaultLog = new QAction(QIcon::fromTheme("view-refresh"), tr("&Reload default log"), this);
    loadCustomLog = new QAction(QIcon::fromTheme("document-open"), tr("&Load custom log"), this);
    openAboutDialog = new QAction(QIcon::fromTheme("help-about"), tr("&About Pacman Log Viewer"), this);
    connect(reloadDefaultLog, SIGNAL(triggered()), SLOT(loadDefaultLog()));
    connect(openAboutDialog, SIGNAL(triggered()), SLOT(showAboutDialog()));
    connect(loadCustomLog, SIGNAL(triggered()), SLOT(selectCustomLogFile()));
    menu->addAction(reloadDefaultLog);
    menu->addAction(loadCustomLog);
    menu->addSeparator();
    menu->addAction(openAboutDialog);
    ui->toolButton->setMenu(menu);

    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);

    DateColumnDelegate *dateDelegate = new DateColumnDelegate;
    ui->tableView->setItemDelegateForColumn(1,dateDelegate);

    ActionColumnDelegate *actionDelegate = new ActionColumnDelegate;
    ui->tableView->setItemDelegateForColumn(2,actionDelegate);

    connect(ui->namesLineEdit, SIGNAL(textChanged(QString)), this, SLOT(applyFilters()));
    connect(ui->FltrsWidget, SIGNAL(filtersChanged()), this, SLOT(applyFilters()));
    connect(ui->exactMatchCheckBox, SIGNAL(toggled(bool)), this, SLOT(applyFilters()));

    loadDefaultLog();
    loadSettings();
}

Dialog::~Dialog()
{
    QSettings settings("PacmanLogViewer", "pacmanlogviewer");
    settings.setValue("Window/x-position", this->x());
    settings.setValue("Window/y-position", this->y());
    settings.setValue("Window/widht", this->width());
    settings.setValue("Window/height", this->height());

    settings.setValue("Table/dateColumnWidth", ui->tableView->columnWidth(1));
    settings.setValue("Table/actionColumnWidth", ui->tableView->columnWidth(2));
    settings.setValue("Table/packageColumnWidth", ui->tableView->columnWidth(3));
    settings.setValue("Table/versionColumnWidth", ui->tableView->columnWidth(4));

    settings.setValue("Filters/ExactMatch", ui->exactMatchCheckBox->isChecked());
    settings.setValue("Filters/ShowInstalled", ui->FltrsWidget->installedChecked());
    settings.setValue("Filters/ShowUpgraded", ui->FltrsWidget->upgradedChecked());
    settings.setValue("Filters/ShowRemoved", ui->FltrsWidget->removedChecked());
    settings.setValue("Filters/ShowDowngraded", ui->FltrsWidget->downgradedChecked());
    settings.setValue("Filters/ShowReinstalled", ui->FltrsWidget->reinstalledChecked());
    settings.setValue("Filters/Interval", ui->FltrsWidget->dateRangeIndex());
    if(ui->FltrsWidget->dateRangeIndex() == 5) {
        settings.setValue("Filters/From", ui->FltrsWidget->fromDate());
        settings.setValue("Filters/To", ui->FltrsWidget->toDate());
    }

    delete ui;
}

void Dialog::loadSettings()
{
    QSettings settings("PacmanLogViewer", "pacmanlogviewer");

    this->setGeometry(settings.value("Window/x-position", 100).toInt(),
                      settings.value("Window/y-position", 100).toInt(),
                      settings.value("Window/widht", 800).toInt(),
                      settings.value("Window/height", 600).toInt());

    ui->tableView->setColumnWidth(1,settings.value("Table/dateColumnWidth", 115).toInt());
    ui->tableView->setColumnWidth(2,settings.value("Table/actionColumnWidth", 55).toInt());
    ui->tableView->setColumnWidth(3,settings.value("Table/packageColumnWidth", 255).toInt());
    ui->tableView->setColumnWidth(4,settings.value("Table/versionColumnWidth",115).toInt());

    ui->exactMatchCheckBox->setChecked(settings.value("Filters/ExactMatch", false).toBool());
    ui->FltrsWidget->setInstallCB(settings.value("Filters/ShowInstalled", true).toBool());
    ui->FltrsWidget->setUpdatedCB(settings.value("Filters/ShowUpgraded", true).toBool());
    ui->FltrsWidget->setRemovedCB(settings.value("Filters/ShowRemoved", true).toBool());
    ui->FltrsWidget->setDowngradedCB(settings.value("Filters/ShowDowngraded", true).toBool());
    ui->FltrsWidget->setReinstalledCB(settings.value("Filters/ShowReinstalled", true).toBool());
    ui->FltrsWidget->setRangeIndex(settings.value("Filters/Interval", 0).toInt());
    if(ui->FltrsWidget->dateRangeIndex() == 5) { // Custom range
        ui->FltrsWidget->setFromDate(settings.value("Filters/From", oldestDate).value<QDate>());
        ui->FltrsWidget->setToDate(settings.value("Filters/To", QDate::currentDate()).value<QDate>());
    }
    else {
        ui->FltrsWidget->setFromDate(oldestDate);
        ui->FltrsWidget->setToDate(QDate::currentDate());
    }
}

void Dialog::readPacmanLogFile(const QString &logFile)
{
    QFile file(logFile);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug() << "can't open log file : " << file.errorString();
        return;
    }
    
    QSqlQuery query("DELETE FROM log");
    query.exec();

    QStringList names;
    QString oldPkg;
    QString oldVer;

    const QRegExp rx("\\[(.+)\\]\\s\\[(PACMAN|ALPM|PACKAGEKIT)\\]\\s(installed|removed|upgraded|downgraded|reinstalled)\\s([\\w-]+)\\s\\((.+)\\)");

    while(!file.atEnd()) {
        QString line = file.readLine();
        
        rx.indexIn(line);
        if(rx.cap(0).isEmpty())
            continue;

        QString timestamp = rx.cap(1);
        const QString who = rx.cap(2);
        const QString op = rx.cap(3);
        const QString pkg = rx.cap(4);
        const QString ver = rx.cap(5);
        
        if(oldPkg == pkg && oldVer == ver)
            continue;
        
        oldPkg = pkg;
        oldVer = ver;

        const int T_ix = timestamp.indexOf("T");
        if (T_ix != -1){
            // New-style timestamp yyyy-MM-ddThh:mm:ss-zzzz. Strip offset and replace
            // the T with a space:
            timestamp.replace(T_ix, 1, " ");
            timestamp = timestamp.left(19);
            
        }
        else{
            // Old style timestamp yyyy-MM-dd hh:mm. Add seconds assuming zero past the
            // minute:
            timestamp.append(":00");
        }

        const QDateTime datetime = QDateTime::fromString(timestamp, "yyyy-MM-dd hh:mm:ss");

        names.append(pkg);

        query.prepare("INSERT INTO log (date,op,pkg,ver) VALUES(:date, :op, :pkg, :ver)");
        query.bindValue( ":date", datetime.toString("yyyy-MM-dd") );
        query.bindValue( ":op", op );
        query.bindValue( ":pkg", pkg );
        query.bindValue( ":ver", ver );
        if(!query.exec())
            qDebug() << query.lastError();
    }
    
    model->select();

    // Populate completer
    names.removeDuplicates();

    if(!names.isEmpty()) {
        pkgNameCompleter->setTags(names);
        pkgNameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    }

    oldestDate = model->data(model->index(0,1)).toDate();
    
    ui->tableView->hideColumn(0);
    ui->tableView->show();
}

void Dialog::applyFilters()
{
    QString filter;
    bool isOR = false;

    int index = ui->FltrsWidget->dateRangeIndex();

    switch(index) {
        case 0:
            filter += "";
            break;
        case 1:
        filter += "( date = '" + QDate::currentDate().toString(Qt::ISODate) + "') AND ";
        break;
    case 2:
        filter += "( date >= '" + QDate::currentDate().addDays(-1).toString(Qt::ISODate) + "') AND ";
        break;
    case 3:
        filter += "( date >= '" + QDate::currentDate().addDays(-6).toString(Qt::ISODate) + "') AND ";
        break;
    case 4:
        filter += "( date >= '" + QDate::currentDate().addMonths(-1).toString(Qt::ISODate) + "') AND ";
        break;
    case 5:
        filter += "( date >= '" + ui->FltrsWidget->fromDate().toString(Qt::ISODate) + "' AND date <= '" + ui->FltrsWidget->toDate().toString(Qt::ISODate) + "') AND ";
        break;
    }

    QStringList pkgNames(ui->namesLineEdit->text().split(","));

    if(pkgNames.count() <=1) {
        if(ui->namesLineEdit->text().isEmpty())
            filter += "pkg like '%" + ui->namesLineEdit->text().remove(",").trimmed() + "%'";
        else if(ui->exactMatchCheckBox->isChecked())
            filter += "pkg='" + ui->namesLineEdit->text().remove(",").trimmed() + "'";
        else
            filter += "pkg like '%" + ui->namesLineEdit->text().remove(",").trimmed() + "%'";
    }
    else if(pkgNames.count() > 1) {
        filter += "(";
        bool isFirst = true;
        foreach(QString pkgName, pkgNames) {
            if(pkgName.trimmed().isEmpty())
                continue;
            if(ui->exactMatchCheckBox->isChecked()) {
                filter += QString(isFirst ? "" : " OR ") + "pkg='" + pkgName.trimmed() + "'";
            }
            else {
                filter += QString(isFirst ? "" : " OR ") + "pkg like '%" + pkgName.trimmed() + "%'";
            }

            isFirst = false;
        }
        filter += ")";
    }

    if(ui->FltrsWidget->installedChecked()) {
        filter += (isOR? " OR " : " AND (") + QString("op='installed'");
        isOR = true;
    }

    if(ui->FltrsWidget->upgradedChecked()) {
        filter += (isOR? " OR " : " AND (") + QString("op='upgraded'");
        isOR = true;
    }

    if(ui->FltrsWidget->removedChecked()) {
        filter += (isOR? " OR " : " AND (") + QString("op='removed'");
        isOR = true;
    }

    if(ui->FltrsWidget->downgradedChecked()) {
        filter += (isOR? " OR " : " AND (") + QString("op='downgraded'");
        isOR = true;
    }

    if(ui->FltrsWidget->reinstalledChecked()) {
        filter += (isOR? " OR " : " AND (") + QString("op='reinstalled'");
        isOR = true;
    }

    if(isOR)
        filter += ")";
    else
        filter += "AND FALSE";

    model->setFilter(filter);
}

void Dialog::showAboutDialog()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void Dialog::selectCustomLogFile()
{
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open Pacman Log File"),
                                                 lastPath.isEmpty() ? QDir::homePath() : lastPath,
                                                 tr("Logs (*.log)"));
     if(!QFile::exists(fileName)) {
         qDebug() << "Selected file" << fileName << "does not exist.";
         return;
     }
     
     lastPath = QFileInfo(fileName).absolutePath();
     
     readPacmanLogFile(fileName);
}

void Dialog::loadDefaultLog()
{
    readPacmanLogFile("/var/log/pacman.log");
}
