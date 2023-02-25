/**
 * SPDX-FileCopyrightText: 2012 Giuseppe Calà <gcala@mailbox.org>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "dialog.h"
#include "ui_dialog.h"
#include "aboutdialog.h"

#include "config.h"
#include "connection.h"
#include "datecolumndelegate.h"
#include "actioncolumndelegate.h"

#include <QSqlTableModel>
#include <QDebug>

#include <QRegularExpression>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    setupTranslations();
    ui->setupUi(this);

    QAction *const quitAction = new QAction(this);
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    addAction(quitAction);

    ui->tableView->setSortingEnabled(true);

    const auto buttonList = this->findChildren<QPushButton *>();
    for(auto pb : buttonList) {
        pb->setDefault( false );
        pb->setAutoDefault( false );
    }

    if(!createConnection())
        return;
    
    QSqlQuery query("CREATE TABLE log (id INTEGER PRIMARY KEY, date TEXT, op TEXT, pkg TEXT, ver TEXT)");
    query.exec();
    
    m_model = new QSqlTableModel;
    m_model->setTable("log");
    m_model->setHeaderData( 1, Qt::Horizontal, tr("Date") );
    m_model->setHeaderData( 2, Qt::Horizontal, tr("Action") );
    m_model->setHeaderData( 3, Qt::Horizontal, tr("Package") );
    m_model->setHeaderData( 4, Qt::Horizontal, tr("Version") );

    ui->tableView->setModel(m_model);
    
    m_pkgNameCompleter = new PkgNameCompleter(ui->namesLineEdit);
    m_pkgNameCompleter->setWidget(ui->namesLineEdit);
    connect(ui->namesLineEdit, QOverload<QStringList, QString>::of(&PkgNamesLineEdit::text_changed), m_pkgNameCompleter, &PkgNameCompleter::update);
    connect(m_pkgNameCompleter,  QOverload<const QString &>::of(&PkgNameCompleter::activated), ui->namesLineEdit, &PkgNamesLineEdit::complete_text);
    
    QMenu *menu = new QMenu(this);
    m_reloadAction = new QAction(QIcon::fromTheme("view-refresh"), tr("&Reload default log"), this);
    m_customLogAction = new QAction(QIcon::fromTheme("document-open"), tr("&Load custom log"), this);
    m_aboutAction = new QAction(QIcon::fromTheme("help-about"), tr("&About Pacman Log Viewer"), this);

    connect(m_reloadAction, &QAction::triggered, this, [=] () {
        readPacmanLogFile("/var/log/pacman.log");
    });

    connect(m_aboutAction, &QAction::triggered, this, [=] () {
        AboutDialog aboutDialog(this);
        aboutDialog.exec();
    });

    connect(m_customLogAction, &QAction::triggered, this, &Dialog::selectCustomLogFile);

    menu->addAction(m_reloadAction);
    menu->addAction(m_customLogAction);
    menu->addSeparator();
    menu->addAction(m_aboutAction);
    ui->toolButton->setMenu(menu);

    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);

    DateColumnDelegate *dateDelegate = new DateColumnDelegate;
    ui->tableView->setItemDelegateForColumn(1,dateDelegate);

    ActionColumnDelegate *actionDelegate = new ActionColumnDelegate;
    ui->tableView->setItemDelegateForColumn(2,actionDelegate);

    connect(ui->namesLineEdit, &QLineEdit::textChanged, this, &Dialog::applyFilters);
    connect(ui->FltrsWidget, &FiltersWidget::filtersChanged, this, &Dialog::applyFilters);
    connect(ui->exactMatchCheckBox, &QCheckBox::toggled, this, &Dialog::applyFilters);

    readPacmanLogFile("/var/log/pacman.log");
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
        ui->FltrsWidget->setFromDate(settings.value("Filters/From", m_oldestDate).value<QDate>());
        ui->FltrsWidget->setToDate(settings.value("Filters/To", QDate::currentDate()).value<QDate>());
    }
    else {
        ui->FltrsWidget->setFromDate(m_oldestDate);
        ui->FltrsWidget->setToDate(QDate::currentDate());
    }
}

QString substLang(const QString &lang)
{
    // Map the more appropriate script codes
    // to country codes as used by Qt and
    // transifex translation conventions.
    
    // Simplified Chinese
    if (lang == QLatin1String("zh_Hans")) {
        return QLatin1String("zh_CN");
    }
    // Traditional Chinese
    if (lang == QLatin1String("zh_Hant")) {
        return QLatin1String("zh_TW");
    }
    return lang;
}

void Dialog::setupTranslations()
{
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    
    auto *translator = new QTranslator(this);
    auto *qtTranslator = new QTranslator(this);
    auto *qtkeychainTranslator = new QTranslator(this);
    
    for (QString lang : qAsConst(uiLanguages)) {
        lang.replace(QLatin1Char('-'), QLatin1Char('_')); // work around QTBUG-25973
        lang = substLang(lang);
        const QString trPath = QString::fromLatin1(SHAREDIR "/" APPLICATION_EXECUTABLE "/i18n/");
        const QString trFile = QLatin1String("pacmanlogviewer_") + lang;
        if (translator->load(trFile, trPath) || lang.startsWith(QLatin1String("en"))) {
            // Permissive approach: Qt and keychain translations
            // may be missing, but Qt translations must be there in order
            // for us to accept the language. Otherwise, we try with the next.
            // "en" is an exception as it is the default language and may not
            // have a translation file provided.
            qDebug() << "Using" << lang << "translation";
            setProperty("ui_lang", lang);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            const QString qtTrPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#else
            const QString qtTrPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
#endif
            const QString qtTrFile = QLatin1String("qt_") + lang;
            const QString qtBaseTrFile = QLatin1String("qtbase_") + lang;
            if (!qtTranslator->load(qtTrFile, qtTrPath)) {
                if (!qtTranslator->load(qtTrFile, trPath)) {
                    if (!qtTranslator->load(qtBaseTrFile, qtTrPath)) {
                        qtTranslator->load(qtBaseTrFile, trPath);
                    }
                }
            }
            const QString qtkeychainTrFile = QLatin1String("qtkeychain_") + lang;
            if (!qtkeychainTranslator->load(qtkeychainTrFile, qtTrPath)) {
                qtkeychainTranslator->load(qtkeychainTrFile, trPath);
            }
            if (!translator->isEmpty())
                QCoreApplication::installTranslator(translator);
            if (!qtTranslator->isEmpty())
                QCoreApplication::installTranslator(qtTranslator);
            if (!qtkeychainTranslator->isEmpty())
                QCoreApplication::installTranslator(qtkeychainTranslator);
            break;
        }
        if (property("ui_lang").isNull()) {
            setProperty("ui_lang", "C");
        }
    }
}

void Dialog::readPacmanLogFile(const QString &logFile)
{
    QFile file(logFile);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug() << "can't open log file : " << file.errorString();
        return;
    }

    QElapsedTimer timer;
    timer.start();

    QSqlQuery query(QLatin1String("DELETE FROM log"));
    query.exec();
    query.prepare(QLatin1String("INSERT INTO log (date,op,pkg,ver) VALUES(:date, :op, :pkg, :ver)"));

    QStringList names;
    QString oldPkg;
    QString oldVer;

    const QString regexString("\\[(.+)\\]\\s\\[(PACMAN|ALPM|PACKAGEKIT)\\]\\s(installed|removed|upgraded|downgraded|reinstalled)\\s([\\w-]+)\\s\\((.+)\\)");

    static QRegularExpression regex(regexString);

    while(!file.atEnd()) {
        const auto line = file.readLine();
        const auto match = regex.match(line);
        if (!match.hasMatch())
            continue;
        auto timestamp = match.captured(1);
        // (unused) const auto who = match.captured(2);
        const auto op = match.captured(3);
        const auto pkg = match.captured(4);
        const auto ver = match.captured(5);
        
        if(oldPkg == pkg && oldVer == ver)
            continue;
        
        oldPkg = pkg;
        oldVer = ver;

        names.append(pkg);

#define PARSE_TIME 0 // QDateTime operations are very slow and are not useful in the current implementation
#if PARSE_TIME
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
        query.bindValue( ":date", datetime.toString("yyyy-MM-dd") );
#else
        timestamp.truncate(10); // leave only yyyy-MM-dd in timestamp
        query.bindValue( ":date", timestamp );
#endif

        query.bindValue( ":op", op );
        query.bindValue( ":pkg", pkg );
        query.bindValue( ":ver", ver );
        if(!query.exec())
            qDebug() << query.lastError();
    }
    
    m_model->select();

    // Populate completer
    names.removeDuplicates();

    if(!names.isEmpty()) {
        m_pkgNameCompleter->setTags(names);
        m_pkgNameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    }

    m_oldestDate = m_model->data(m_model->index(0,1)).toDate();
    
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

    const QStringList pkgNames(ui->namesLineEdit->text().split(","));

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
        for(const auto &pkgName : pkgNames) {
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

    m_model->setFilter(filter);
}

void Dialog::selectCustomLogFile()
{
     const QString fileName = QFileDialog::getOpenFileName(this, tr("Open Pacman Log File"),
                                                 m_lastPath.isEmpty() ? QDir::homePath() : m_lastPath,
                                                 tr("Logs (*.log)"));
     if(!QFile::exists(fileName)) {
         qDebug() << "Selected file" << fileName << "does not exist.";
         return;
     }
     
     m_lastPath = QFileInfo(fileName).absolutePath();
     
     readPacmanLogFile(fileName);
}
