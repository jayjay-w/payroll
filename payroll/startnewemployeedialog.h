#ifndef STARTNEWEMPLOYEEDIALOG_H
#define STARTNEWEMPLOYEEDIALOG_H

#include <QDialog>

namespace Ui {
class StartNewEmployeeDialog;
}

class StartNewEmployeeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit StartNewEmployeeDialog(QWidget *parent = 0);
	~StartNewEmployeeDialog();
	QString empID;
private slots:
	void on_cmdSaveAndContinue_clicked();

private:
	Ui::StartNewEmployeeDialog *ui;
};

#endif // STARTNEWEMPLOYEEDIALOG_H
