#ifndef PAYROLLMAINWINDOW_H
#define PAYROLLMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class PayrollMainWindow;
}

class PayrollMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit PayrollMainWindow(QWidget *parent = 0);
	~PayrollMainWindow();

private:
	Ui::PayrollMainWindow *ui;
};

#endif // PAYROLLMAINWINDOW_H
