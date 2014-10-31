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
signals:
	void dataChanged();
private slots:
	void on_cmdSave_clicked();

private:
	Ui::CompanyInformationDialog *ui;
};

#endif // COMPANYINFORMATIONDIALOG_H
