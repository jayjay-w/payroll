#ifndef PUBLICS_H
#define PUBLICS_H

#include <QObject>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QComboBox>
#include "newcompanydialog.h"

class Publics : public QObject
{
	Q_OBJECT
public:
	explicit Publics(QObject *parent = 0);
	enum SQL_STRING {
		SQL_COMPANY_TABLE,
		SQL_EMPLOYEES_TABLE,
		SQL_YEARS,
		SQL_MONTHNAMES,
		SQL_PAYROLLMONTHS,
		SQL_PAY_TYPES
	};

	static QString lastPath();
	static QString getOpenFile(QWidget *parent = 0, QString filter = "Payroll Files (*.pay)");
	static QString getSaveFile(QWidget *parent = 0, QString filter = "Payroll Files (*.pay)");
	static void saveSetting(QString settingName = "", QVariant value = 0);
	static QVariant getSetting(QString settingName = "", QVariant defaultValue = 0);
	static QString getSql(SQL_STRING sqlString);
	static QVariant getDbValue(QString query, QString returnCol);
	static QVariant loadQueryToCombo(QString query = "SELECT NOW()", QString col = "NOW", QComboBox *cbo = 0);
signals:

public slots:

};

#endif // PUBLICS_H
