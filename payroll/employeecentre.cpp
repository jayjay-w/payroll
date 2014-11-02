#include "employeecentre.h"
#include "ui_employeecentre.h"

#include <QtSql>
#include "publics.h"
#include "payrollmainwindow.h"

EmployeeCentre::EmployeeCentre(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EmployeeCentre)
{
	ui->setupUi(this);
}

EmployeeCentre::~EmployeeCentre()
{
	delete ui;
}

void EmployeeCentre::setEmployeeID(QString empID)
{
	currentEmployeeId = empID;
	for (int i = 0; i < ui->trvEmployees->invisibleRootItem()->childCount(); i++) {
		QTreeWidgetItem *it = ui->trvEmployees->invisibleRootItem()->child(i);
		QFont fnt = it->font(0);
		fnt.setBold(false);
		if (it->text(99) == empID) {
			fnt.setBold(true);
			ui->trvEmployees->scrollToItem(it);
		}
		it->setFont(0, fnt);
	}
	reloadEmployeeDetails();
}

void EmployeeCentre::reloadEmployeeDetails()
{
	Publics::clearTextBoxes(this);

	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM Employees WHERE EmployeeID = '" + currentEmployeeId + "'");
	if (qu.lastError().isValid()){
		PayrollMainWindow::instance()->showQueryError(qu);
		return;
	}

	//All Ok
	qu.first();
	QSqlRecord rec  = qu.record();

	ui->txtFirstName->setText(rec.value("FirstName").toString());
	ui->txtMiddleName->setText(rec.value("MiddleName").toString());
	ui->txtLastName->setText(rec.value("LastName").toString());
}

void EmployeeCentre::reloadEmployeeList()
{
	ui->trvEmployees->invisibleRootItem()->takeChildren();

	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM Employees");
	if (qu.lastError().isValid()){
		PayrollMainWindow::instance()->showQueryError(qu);
		return;
	}

	//All Ok
	while (qu.next()) {
		QString id = qu.record().value("EmployeeID").toString();
		QString combinedName = qu.record().value("FirstName").toString() + " "
				+ qu.record().value("MiddleName").toString() + " "
				+ qu.record().value("LastName").toString();

		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvEmployees);
		it->setText(0, combinedName);
		it->setText(99, id);

		if (id == currentEmployeeId) {
			//This is the current employee
			QFont fnt = it->font(0);
			fnt.setBold(true);
			it->setFont(0, fnt);
			ui->trvEmployees->scrollToItem(it);
		}
	}

	ui->trvEmployees->resizeColumnToContents(0);
}

void EmployeeCentre::on_trvEmployees_itemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	setEmployeeID(item->text(99));
}
