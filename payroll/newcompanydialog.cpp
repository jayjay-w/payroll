#include "newcompanydialog.h"
#include "ui_newcompanydialog.h"
#include "publics.h"
NewCompanyDialog::NewCompanyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCompanyDialog)
{
    ui->setupUi(this);
}

NewCompanyDialog::~NewCompanyDialog()
{
    delete ui;
}

void NewCompanyDialog::on_cmdClose_clicked()
{
    if (QMessageBox::question(this, "Confirm Close", "Are you sure you want to cancel this process?"
                              , QMessageBox::Yes, QMessageBox::No
                              ) == QMessageBox::Yes) {
        this->reject();
    }

}

void NewCompanyDialog::on_cmdSetFilePath_clicked()
{
    Publics pub;
    QString filePath = pub.getSaveFile(this);
    if (filePath.length() > 0)
        ui->txtFileLocation->setText(filePath);

    pub.deleteLater();
}

void NewCompanyDialog::on_cmdCreateCompany_clicked()
{
    if (ui->txtCompanyName->text().length() < 1) {
        QMessageBox::critical(this, "Error", "Please specify the company name.");
        return;
    }

    if (ui->txtFileLocation->text().length() < 1) {
        QMessageBox::critical(this, "Error", "Please specify the company file location.");
        return;
    }

    //ALL OK
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "newCompanyFileDb");
    db.setDatabaseName(ui->txtFileLocation->text());
    if (!db.open()) {
        QMessageBox::critical(this, "Error", QString("The following error occurred while creating your company file:\n%1.\nPlease contact your administrator.")
                              .arg(db.lastError().text()));
        return;
    }

    db.exec(Publics::getSql(Publics::SQL_COMPANY_TABLE));

    if (!db.lastError().isValid()) {
        QString insQu = "INSERT INTO Company(CompanyName, NSSF, NHIF, PinNo, VATNo, Address, Postcode, Town, Mobile"
                ",Tel1, Tel2, Email, Fax, Website) VALUES ("
                "'" + ui->txtCompanyName->text() + "', "
                "'" + ui->txtNSSFNo->text() + "', "
                "'" + ui->txtNHIFNo->text() + "', "
                "'" + ui->txtPINNo->text() + "', "
                "'" + ui->txtVATNo->text() + "', "
                "'" + ui->txtAddress->toPlainText() + "', "
                "'" + ui->txtPostcode->text() + "', "
                "'" + ui->txtTown->text() + "', "
                "'" + ui->txtMobile->text() + "', "
                "'" + ui->txtTel1->text() + "', "
                "'" + ui->txtTel2->text() + "', "
                "'" + ui->txtEmail->text() + "', "
                "'" + ui->txtFax->text() + "', "
                "'" + ui->txtWebsite->text() + "')";
        db.exec(insQu);
        if (!db.lastError().isValid()) {
            companyFileLocation = ui->txtFileLocation->text();
            db.close();
            this->accept();
        } else {
            QMessageBox::critical(this, "Error", QString("The following error occurred while creating your company file:\n%1.\nPlease contact your administrator.")
                                  .arg(db.lastError().text()));
            return;
        }
    } else {
        QMessageBox::critical(this, "Error", QString("The following error occurred while creating your company file:\n%1.\nPlease contact your administrator.")
                              .arg(db.lastError().text()));
        return;
    }
}
