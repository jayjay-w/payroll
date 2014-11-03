#ifndef EMPLOYEEEDITOR_H
#define EMPLOYEEEDITOR_H

#include <QWidget>
#include <QAction>

namespace Ui {
class EmployeeEditor;
}

class EmployeeEditor : public QWidget
{
	Q_OBJECT

public:
	enum EditMode {
		NONE,
		SINGLE_EMPLOYEE_EDIT,
		SINGLE_EMPLOYEE_DISPLAY,
		PAYMENTS,
		ADD,
		IGNORE
	};

	explicit EmployeeEditor(QWidget *parent = 0);
	~EmployeeEditor();
	bool isEditing();

public slots:
	void acceptChanges();
	void rejectChanges();
	void showEmployeeDetails(bool clear);
	void hideMessage();
	void closeMessage();
	void displayMessage(QString str);
	void enableEdition(EditMode newEditMode = NONE);
	void resetPalette();
	void clearEmployee();
signals:
	void addEmployeeFinished();
	void editStatus(bool);
	void employeeChanged();
private slots:
	void lineEditTextChanged(const QString &arg1);

private:
	Ui::EmployeeEditor *ui;
	bool modified;
	EditMode editMode;
	void markChangedWidget(QWidget *w);

	void addMessageAction(QAction *action);
};

#endif // EMPLOYEEEDITOR_H
