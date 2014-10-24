#include "payrollmainwindow.h"
#include "ui_payrollmainwindow.h"

PayrollMainWindow::PayrollMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::PayrollMainWindow)
{
	ui->setupUi(this);
}

PayrollMainWindow::~PayrollMainWindow()
{
	delete ui;
}
