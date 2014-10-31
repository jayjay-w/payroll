#include "employeecentre.h"
#include "ui_employeecentre.h"

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
