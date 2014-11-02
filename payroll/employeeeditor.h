#ifndef EMPLOYEEEDITOR_H
#define EMPLOYEEEDITOR_H

#include <QWidget>

namespace Ui {
class EmployeeEditor;
}

class EmployeeEditor : public QWidget
{
	Q_OBJECT

public:
	explicit EmployeeEditor(QWidget *parent = 0);
	~EmployeeEditor();

private:
	Ui::EmployeeEditor *ui;
};

#endif // EMPLOYEEEDITOR_H
