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
	case SQL_COMPANY:
		return "CREATE TABLE IF NOT EXISTS 'company' ('CompanyName' Text, 'NSSF' Text, 'NHIF' Text, 'PinNo' Text, 'VATNo' Text,"
		       "'Address' Text, 'PostCode' Text, 'Town' Text, 'Mobile' Text, 'Tel1' Text, 'Tel2' Text, 'Fax' Text,"
		       "'Email' Text, 'Website' Text, 'appName' Text DEFAULT('smpayroll'), 'CurrentMonth' Integer, 'dbVersion' TEXT DEFAULT('1.0'))";
	case SQL_MONTHNAMES:
		return "CREATE TABLE IF NOT EXISTS 'MonthNames' ('MonthID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
		       "'MonthNo' Integer, 'MonthName' Text)";
	case SQL_YEARS:
		return "CREATE TABLE IF NOT EXISTS 'Years' ('YearID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'Year' INTEGER)";
	case SQL_PAYROLLMONTHS:
		return "CREATE TABLE IF NOT EXISTS 'PayrollMonths' ('PayrollMonthID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
		       "'YearID' INTEGER, 'MonthID' INTEGER, 'Year' TEXT, "
		       "'Month' TEXT, 'Closed' INTEGER, LastEdited TEXT)";
	case SQL_PAY_TYPES:
		return "CREATE TABLE IF NOT EXISTS 'PayTypes' ('PayTypeID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'PayCategory' TEXT, PayType TEXT)";
	case SQL_EMPLOYEES:
		return "CREATE TABLE IF NOT EXISTS 'Employees' ('EmployeeID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'FirstName' TEXT, 'MiddleName' TEXT, 'LastName' TEXT, "
		       "'IDNo' TEXT, 'PINNo' TEXT, 'NHIFNo' TEXT, 'NSSFNo' TEXT, "
		       "'IsActive' TEXT DEFAULT('1'), 'GroupID' INTEGER, 'DepartmentID' INTEGER,"
		       "'PhoneNo' TEXT, 'Email' TEXT, 'Address' TEXT, 'Postcode' TEXT, 'Town' TEXT,"
		       "'Photo' BLOB)";
	case SQL_DEPARTMENTS:
		return "CREATE TABLE IF NOT EXISTS 'Departments' ('DepartmentID'  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				"'Department' TEXT)";
	case SQL_JOB_GROUPS:
		return "CREATE TABLE IF NOT EXISTS 'JobGroups' ('JobGroupID'  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
				"'JobGroup' TEXT)";
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
