#include "companyinitializationdialog.h"
#include "ui_companyinitializationdialog.h"
#include "publics.h"
CompanyInitializationDialog::CompanyInitializationDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CompanyInitializationDialog)
{
	ui->setupUi(this);
	DatabaseInitThread *th = new DatabaseInitThread(this);
	connect (th, SIGNAL(progress(int))
		 , ui->prgProgress, SLOT(setValue(int)));
	connect (th, SIGNAL(status(QString))
		 , ui->lblCurrentAction, SLOT(setText(QString)));
	connect (th, SIGNAL(finished())
		 , SLOT(threadFinished()));

	th->start();
}

CompanyInitializationDialog::~CompanyInitializationDialog()
{
	delete ui;
}

void CompanyInitializationDialog::threadFinished()
{
	this->accept();
}


DatabaseInitThread::DatabaseInitThread(QObject *parent) :
	QThread(parent)
{

}

void DatabaseInitThread::run()
{
	QSqlDatabase db = QSqlDatabase::database();
	//Create tables
	executeInitSql(Publics::getSql(Publics::SQL_MONTHNAMES));
	executeInitSql(Publics::getSql(Publics::SQL_YEARS));
	executeInitSql(Publics::getSql(Publics::SQL_PAYROLLMONTHS));
	executeInitSql(Publics::getSql(Publics::SQL_PAY_TYPES));
	executeInitSql(Publics::getSql(Publics::SQL_DEPARTMENTS));
	executeInitSql(Publics::getSql(Publics::SQL_JOB_GROUPS));
	executeInitSql(Publics::getSql(Publics::SQL_EMPLOYEES));
	executeInitSql(Publics::getSql(Publics::SQL_PAYMENTS));

	int minYear = QDate::currentDate().year() - 1;
	int maxYear = minYear + 5;
	//Prepare Years
	QSqlQuery qu = db.exec("SELECT Count(YearID) as 'Cnt' FROM Years");
	if (!qu.lastError().isValid()) {
		emit status("Initializing years");
		//No error
		qu.first();
		int cnt = qu.record().value("cnt").toInt();
		if (cnt < 1) {
			//Insert years
			int step = 0;
			for (int i = minYear; i < maxYear; i++) {
				step ++;
				db.exec("INSERT INTO Years (Year) VALUES('" + QString::number(i) + "')");
				if (db.lastError().isValid()) {
					qDebug() << db.lastError().text();
					return;
				}
				int prg = step * 20;
				emit progress(prg);
				emit status("Initializing years: " + QString::number(i));
			}
		}
	}

	emit progress(100);
	//Prepare month names
	qu = db.exec("SELECT Count(MonthID) as 'Cnt' FROM MonthNames");
	if (!qu.lastError().isValid()) {
		emit status("Initializing month names");
		qu.first();
		int cnt = qu.record().value("cnt").toInt();
		if (cnt < 1) {
			emit progress(0);
			//Insert months
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('1', 'January')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('2', 'February')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('3', 'March')");
			emit progress(25);
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('4', 'April')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('5', 'May')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('6', 'June')");
			emit progress(50);
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('7', 'July')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('8', 'August')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('9', 'September')");
			emit progress(75);
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('10', 'October')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('11', 'November')");
			db.exec("INSERT INTO MonthNames (MonthNo, MonthName) VALUES ('12', 'December')");
			emit progress(100);
		}
	}
	emit progress(100);
	//Insert PayrollMonths
	//Loop through all 200 years. For each year, loop through each month and add a payroll month.
	qu = db.exec("SELECT Count(PayrollMonthID) as 'Cnt' FROM PayrollMonths");
	if (!qu.lastError().isValid()) {
		emit status("Creating payroll periods");
		qu.first();
		int cnt = qu.record().value("cnt").toInt();
		if (cnt < 1) {
			emit progress(0);
			qu = db.exec("SELECT * FROM Years");
			int step = 0;
			while (qu.next()) {
				step++;
				QString yearID = qu.record().value("YearID").toString();
				QString yearName = qu.record().value("Year").toString();
				QSqlQuery qu2 = db.exec("SELECT * FROM MonthNames");
				while (qu2.next()) {
					QString monthID = qu2.record().value("MonthID").toString();
					QString monthName = qu2.record().value("MonthName").toString();

					db.exec("INSERT INTO PayrollMonths (YearID, MonthID, Year, Month) VALUES ('"
						+ yearID + "', '" + monthID + "', '" + yearName + "', '" + monthName + "')");
					emit status("Creating payroll periods: " + yearName + " " + monthName);
				}
				emit progress(step * 20);
			}
		}
	}
	emit progress(100);
	//Insert pay categories
	qu = db.exec("SELECT Count(PayTypeID) as 'cnt' FROM PayTypes");
	if (!qu.lastError().isValid()) {
		qu.first();
		int cnt = qu.record().value("cnt").toInt();
		if (cnt < 1) {
			//No pay categories
			emit status("Creating pay categories");
			//Insert pays
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Basic')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Allowance')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Bonus')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Commission')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'House Allowance')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Leave')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Overtime')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Sick')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Pension')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Pay', 'Damages')");
			//Insert benefits
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'Car')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'Electricity')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'House')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'Servant')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'Phone')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'Water')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'Fringe')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Benefit', 'Furniture')");
			//Insert Allowed deductions
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Allowed Ded', 'Home Interest')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Allowed Ded', 'Lateness')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Allowed Ded', 'Home Saving')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Allowed Ded', 'Pension')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Allowed Ded', 'Premium')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Allowed Ded', 'Voluntary NSSF')");
			//Insert Reliefs
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Relief', 'Insurance')");
			//Insert Deductions
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'HELB')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Coop Loan')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Coop Shares')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Extended Advance')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Normal Advance')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Cash Advance')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Lateness')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Company Loan')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Loan Interest')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Pension')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Voluntary NSSF')");
			db.exec("INSERT INTO PayTypes (PayCategory, PayType) VALUES ('Deduction', 'Welfare')");
		}
	}
}

void DatabaseInitThread::executeInitSql(QString sql)
{
	QSqlDatabase db = QSqlDatabase::database();
	//Create tables
	db.exec(sql);
	if (db.lastError().isValid())
		qDebug() << db.lastError().text();
}
