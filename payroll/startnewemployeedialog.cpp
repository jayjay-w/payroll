#include "startnewemployeedialog.h"
#include "ui_startnewemployeedialog.h"

#include <QtSql>
#include "payrollmainwindow.h"

StartNewEmployeeDialog::StartNewEmployeeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StartNewEmployeeDialog)
{
	ui->setupUi(this);
}

StartNewEmployeeDialog::~StartNewEmployeeDialog()
{
	delete ui;
}

void StartNewEmployeeDialog::on_cmdSaveAndContinue_clicked()
{
	QSqlQuery qu = QSqlDatabase::database().exec("INSERT INTO Employees(FirstName, MiddleName, LastName) VALUES('"
						     + ui->txtFirstName->text() + "', '"
						     + ui->txtMiddleName->text() + "', '"
						     + ui->txtLastName->text() + "')");
	if (qu.lastError().isValid()) {
		PayrollMainWindow::instance()->showQueryError(qu);
	} else {
		this->accept();
	}
}
