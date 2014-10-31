#ifndef PAYTYPESDIALOG_H
#define PAYTYPESDIALOG_H

#include <QDialog>
#include "sqlmodels.h"
namespace Ui {
class PayTypesDialog;
}

class PayTypesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PayTypesDialog(QWidget *parent = 0);
	~PayTypesDialog();

private slots:
    void on_cmdNew_clicked();

    void on_cmdSave_clicked();

    void on_cmdDelete_clicked();

private:
	Ui::PayTypesDialog *ui;
    SqlTableModel *paysModel;
};

#endif // PAYTYPESDIALOG_H
