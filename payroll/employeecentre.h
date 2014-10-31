#ifndef EMPLOYEECENTRE_H
#define EMPLOYEECENTRE_H

#include <QDialog>

namespace Ui {
class EmployeeCentre;
}

class EmployeeCentre : public QDialog
{
	Q_OBJECT

public:
	explicit EmployeeCentre(QWidget *parent = 0);
	~EmployeeCentre();

private:
	Ui::EmployeeCentre *ui;
};

#endif // EMPLOYEECENTRE_H
