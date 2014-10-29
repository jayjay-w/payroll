#include "paytypesdialog.h"
#include "ui_paytypesdialog.h"

PayTypesDialog::PayTypesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PayTypesDialog)
{
	ui->setupUi(this);
}

PayTypesDialog::~PayTypesDialog()
{
	delete ui;
}
