#ifndef COMPANYINFORMATIONDIALOG_H
#define COMPANYINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CompanyInformationDialog;
}

class CompanyInformationDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CompanyInformationDialog(QWidget *parent = 0);
	~CompanyInformationDialog();

private:
	Ui::CompanyInformationDialog *ui;
};

#endif // COMPANYINFORMATIONDIALOG_H
