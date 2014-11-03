#include "publics.h"
#include <QSettings>

#include <QLineEdit>
#include <QDebug>

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
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	sett.setValue(settingName, value);
}

QVariant Publics::getSetting(QString settingName, QVariant defaultValue)
{
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
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
	case SQL_PAYMENTS:
		return "CREATE TABLE IF NOT EXISTS 'Payments' ('PaymentID'  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, "
				"'EmployeeID' INTEGER, 'PayrollMonthID' INTEGER, 'Amount' FLOAT DEFAULT(0), 'Recurring' INTEGER DEFAULT(0),"
				"'DateAdded' Text)";
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

void Publics::loadQueryToCombo(QString query, QString col, QComboBox *cbo, QVariant userData)
{
	cbo->clear();
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery qu = db.exec(query);
	while (qu.next()) {
		cbo->addItem(qu.record().value(col).toString(), userData);
	}
}

///
/// \brief Publics::setComboBoxText
/// useful for pre Qt5 versions that dont have QComboBox::setCurrentText()
/// \param cbo
/// \param text
///
void Publics::setComboBoxText(QComboBox *cbo, QString text)
{
#if QT_VERSION < 0x50000
	for (int i = 0; i < cbo->count(); i++) {
		if (cbo->itemText(i) == text) {
			cbo->setCurrentIndex(i);
			return;
		}
	}
#endif
#if QT_VERSION > 0x50000
	cbo->setCurrentText(text);
#endif
}

void Publics::clearTextBoxes(QWidget *parent)
{
	for (int i = 0; i < parent->children().count(); i++) {
		if (qobject_cast<QLineEdit *>(parent->children().at(i))) {
			QLineEdit *txt = qobject_cast<QLineEdit *>(parent->children().at(i));
			txt->setText("");
		}
		if (parent->children().at(i)->children().count() > 0) {
			if (qobject_cast<QWidget *>(parent->children().at(i))) {
				QWidget *wid = qobject_cast<QWidget *>(parent->children().at(i));
				if (wid->children().count() > 0) {
					clearTextBoxes(wid);
				}
			}
		} else {
			if (qobject_cast<QLineEdit *>(parent->children().at(i))) {
				QLineEdit *txt = qobject_cast<QLineEdit *>(parent->children().at(i));
				txt->setText("");
			}
		}
	}
}
