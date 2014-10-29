#include "publics.h"
#include <QSettings>
Publics::Publics(QObject *parent) :
    QObject(parent)
{
}

QString Publics::lastPath()
{
    return getSetting("lastPath", QDir::homePath()).toString();
}

QString Publics::getOpenFile(QWidget *parent, QString filter)
{
    QFileDialog dg;
    QString ret = dg.getOpenFileName(parent, "New File Name", lastPath(), filter);
    if (ret.length() > 0) {
        saveSetting("lastPath", ret);
    }
    return ret;
}

QString Publics::getSaveFile(QWidget *parent, QString filter)
{
    QFileDialog dg;
    QString ret = dg.getSaveFileName(parent, "New File Name", lastPath(), filter);
    if (ret.length() > 0) {
        saveSetting("lastPath", ret);
    }
    return ret;
}

void Publics::saveSetting(QString settingName, QVariant value)
{
    QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
    sett.setValue(settingName, value);
}

QVariant Publics::getSetting(QString settingName, QVariant defaultValue)
{
    QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
    return sett.value(settingName, defaultValue);
}

QString Publics::getSql(SQL_STRING sqlString)
{
    switch (sqlString) {
        case SQL_COMPANY_TABLE:
            return "CREATE TABLE IF NOT EXISTS 'company' ('CompanyName' Text, 'NSSF' Text, 'NHIF' Text, 'PinNo' Text, 'VATNo' Text,"
                    "'Address' Text, 'PostCode' Text, 'Town' Text, 'Mobile' Text, 'Tel1' Text, 'Tel2' Text, 'Fax' Text,"
                    "'Email' Text, 'Website' Text, 'appName' Text DEFAULT('smpayroll'), 'CurrentMonth' Integer)";
        default:
            return "";
    }
}

QVariant Publics::getDbValue(QString query, QString returnCol)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery qu = db.exec(query);
    qu.first();
    return qu.record().value(returnCol);
}

QVariant Publics::loadQueryToCombo(QString query, QString col, QComboBox *cbo)
{
    cbo->clear();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery qu = db.exec(query);
    while (qu.next()) {
        cbo->addItem(qu.record().value(col).toString());
    }
}
