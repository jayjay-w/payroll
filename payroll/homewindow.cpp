#include "homewindow.h"
#include "ui_homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::HomeWindow)
{
	ui->setupUi(this);
}

HomeWindow::~HomeWindow()
{
	delete ui;
}
