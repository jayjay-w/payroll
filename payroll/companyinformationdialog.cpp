#include "companyinformationdialog.h"
#include "ui_companyinformationdialog.h"

#include <QtSql>
#include <QMessageBox>
CompanyInformationDialog::CompanyInformationDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CompanyInformationDialog)
{
	ui->setupUi(this);
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * From Company");

	if (!qu.lastError().isValid()) {
		qu.first();
		ui->txtCompanyName->setText(qu.record().value("CompanyName").toString());
		ui->txtNSSFNo->setText(qu.record().value("NSSF").toString());
		ui->txtNHIFNo->setText(qu.record().value("NHIF").toString());
		ui->txtPINNo->setText(qu.record().value("PinNo").toString());
		ui->txtVATNo->setText(qu.record().value("VATNo").toString());
		ui->txtAddress->setPlainText(qu.record().value("Address").toString());
		ui->txtPostcode->setText(qu.record().value("Postcode").toString());
		ui->txtTown->setText(qu.record().value("Town").toString());
		ui->txtMobile->setText(qu.record().value("Mobile").toString());
		ui->txtTel1->setText(qu.record().value("Tel1").toString());
		ui->txtTel2->setText(qu.record().value("Tel2").toString());
		ui->txtFax->setText(qu.record().value("Fax").toString());
		ui->txtEmail->setText(qu.record().value("Email").toString());
		ui->txtWebsite->setText(qu.record().value("Website").toString());
	}
}

CompanyInformationDialog::~CompanyInformationDialog()
{
	delete ui;
}

void CompanyInformationDialog::on_cmdSave_clicked()
{
	QString query = "UPDATE company SET "
			"CompanyName = '" + ui->txtCompanyName->text() + "', "
			"NSSF = '" + ui->txtNSSFNo->text() + "', "
			"NHIF = '" + ui->txtNHIFNo->text() + "', "
			"PinNo = '" + ui->txtPINNo->text() + "', "
			"VATNo = '" + ui->txtVATNo->text() + "', "
			"Address = '" + ui->txtAddress->toPlainText() + "', "
			"Postcode = '" + ui->txtPostcode->text() + "', "
			"Town = '" + ui->txtTown->text() + "', "
			"Mobile = '" + ui->txtMobile->text() + "', "
			"Tel1 = '" + ui->txtTel1->text() + "', "
			"Tel2 = '" + ui->txtTel2->text() + "', "
			"Fax = '" + ui->txtFax->text() + "', "
			"Email = '" + ui->txtEmail->text() + "', "
			"Website = '" + ui->txtWebsite->text() + "'"
			;

	QSqlQuery qu = QSqlDatabase::database().exec(query);

	if (!qu.lastError().isValid()) {
		emit dataChanged();
	} else {
		QMessageBox::critical(this, "Error", qu.lastError().text());
	}
}
