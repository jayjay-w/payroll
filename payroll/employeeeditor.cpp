#include "employeeeditor.h"
#include "ui_employeeeditor.h"

EmployeeEditor::EmployeeEditor(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::EmployeeEditor)
{
	ui->setupUi(this);
}

EmployeeEditor::~EmployeeEditor()
{
	delete ui;
}
