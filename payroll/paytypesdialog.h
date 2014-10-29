#ifndef PAYTYPESDIALOG_H
#define PAYTYPESDIALOG_H

#include <QDialog>

namespace Ui {
class PayTypesDialog;
}

class PayTypesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PayTypesDialog(QWidget *parent = 0);
	~PayTypesDialog();

private:
	Ui::PayTypesDialog *ui;
};

#endif // PAYTYPESDIALOG_H
