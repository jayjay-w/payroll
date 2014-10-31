#include "companyinformationdialog.h"
#include "ui_companyinformationdialog.h"

CompanyInformationDialog::CompanyInformationDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CompanyInformationDialog)
{
	ui->setupUi(this);
}

CompanyInformationDialog::~CompanyInformationDialog()
{
	delete ui;
}
