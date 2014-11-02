#ifndef EMPLOYEECENTRE_H
#define EMPLOYEECENTRE_H

#include <QDialog>

class QTreeWidgetItem;

namespace Ui {
class EmployeeCentre;
}

class EmployeeCentre : public QDialog
{
	Q_OBJECT

public:
	explicit EmployeeCentre(QWidget *parent = 0);
	~EmployeeCentre();
	QString currentEmployeeId;
public slots:
	void setEmployeeID(QString empID);
	void reloadEmployeeDetails();
	void reloadEmployeeList();
private slots:
	void on_trvEmployees_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::EmployeeCentre *ui;
};

#endif // EMPLOYEECENTRE_H
