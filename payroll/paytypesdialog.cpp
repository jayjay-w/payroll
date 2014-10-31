#include "paytypesdialog.h"
#include "ui_paytypesdialog.h"

#include <QMessageBox>
#include <QSqlError>
PayTypesDialog::PayTypesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayTypesDialog)
{
    ui->setupUi(this);
    paysModel = new SqlTableModel(this, QSqlDatabase::database());
    paysModel->setTable("PayTypes");
    paysModel->select();

    paysModel->setHeaderData(1, Qt::Horizontal, "Category");
    paysModel->setHeaderData(2, Qt::Horizontal, "Pay Type");

    ui->tblPayTypesEditor->setModel(paysModel);
    ui->tblPayTypesEditor->setColumnHidden(0, true);
    ui->tblPayTypesEditor->resizeColumnsToContents();
    ui->tblPayTypesEditor->resizeRowsToContents();
}

PayTypesDialog::~PayTypesDialog()
{
    delete ui;
}

void PayTypesDialog::on_cmdNew_clicked()
{
    paysModel->insertRows(paysModel->rowCount(), 1);
    ui->tblPayTypesEditor->resizeRowsToContents();
}

void PayTypesDialog::on_cmdSave_clicked()
{
    ui->tblPayTypesEditor->selectRow(ui->tblPayTypesEditor->currentIndex().row());
    if (!paysModel->submitAll()) {
        int ret = QMessageBox::question(this, qApp->applicationName(),
                                        tr("There is a pending transaction in progress. That cannot be commited now."\
                                           "\nError: %1\n"\
                                           "Perform rollback?").arg(paysModel->lastError().text()),
                                        QMessageBox::Yes, QMessageBox::No);
        if(ret == QMessageBox::Yes)
            paysModel->revertAll();
        paysModel->setPendingTransaction(false);
        ui->tblPayTypesEditor->resizeRowsToContents();
        return;
    }
    ui->tblPayTypesEditor->resizeRowsToContents();
}

void PayTypesDialog::on_cmdDelete_clicked()
{
    if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this item?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        paysModel->removeRows(ui->tblPayTypesEditor->currentIndex().row(), 1);
    }
}
