#include "companyinitializationdialog.h"
#include "ui_companyinitializationdialog.h"

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
                    QString monthID = qu2.value("MonthID").toString();
                    QString monthName = qu2.value("MonthName").toString();

                    db.exec("INSERT INTO PayrollMonths (YearID, MonthID, Year, Month) VALUES ('"
                            + yearID + "', '" + monthID + "', '" + yearName + "', '" + monthName + "')");
                    emit status("Creating payroll periods: " + yearName + " " + monthName);
                }
                emit progress(step * 20);
            }
        }
    }
    emit progress(100);
}
