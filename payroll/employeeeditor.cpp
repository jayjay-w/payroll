#include "employeeeditor.h"
#include "ui_employeeeditor.h"

#include "payrollmainwindow.h"
#include "publics.h"

EmployeeEditor::EmployeeEditor(QWidget *parent) :
	QWidget(parent), editMode(IGNORE),
	ui(new Ui::EmployeeEditor)
{
	ui->setupUi(this);

	QAction *action = new QAction(tr("Save"), this);
	connect (action, SIGNAL(triggered(bool)), this, SLOT(acceptChanges()));
	addMessageAction(action);

	action = new QAction(tr("Cancel"), this);
	connect (action, SIGNAL(triggered(bool)), this, SLOT(rejectChanges()));
	addMessageAction(action);

	closeMessage();

	setEnabled(false);

	ui->txtFirstName->installEventFilter(this);
	ui->txtLastName->installEventFilter(this);
	ui->txtMiddleName->installEventFilter(this);
}

EmployeeEditor::~EmployeeEditor()
{
	delete ui;
}

bool EmployeeEditor::isEditing()
{
	return editMode != NONE;
}

void EmployeeEditor::acceptChanges()
{
	modified = false;
	emit editStatus(false);
	enableEdition(IGNORE);
	closeMessage();
	resetPalette();

	QString query = "UPDATE Employees SET "
			"FirstName = '" + ui->txtFirstName->text() + "', "
			"MiddleName = '" + ui->txtMiddleName->text() + "', "
			"LastName = '" + ui->txtLastName->text() + "'"
			" WHERE EmployeeID = '" + PayrollMainWindow::instance()->currentEmployeeID + "'";

	QSqlQuery qu = QSqlDatabase::database().exec(query);

	if (qu.lastError().isValid()) {
		PayrollMainWindow::instance()->showQueryError(qu);
	}

	emit employeeChanged();
	showEmployeeDetails(false);
}

void EmployeeEditor::rejectChanges()
{
	emit editStatus(false);
	modified = false;
	enableEdition(IGNORE);
	closeMessage();
	resetPalette();
	showEmployeeDetails(false);
}

void EmployeeEditor::showEmployeeDetails(bool clear)
{
	//don't execute this while adding / editing an employee
	if (editMode == ADD || editMode == SINGLE_EMPLOYEE_EDIT)
		return;


	if (!isEnabled() && !clear)
		setEnabled(true);

	if (isEnabled() && clear)
		setEnabled(false);

	editMode = IGNORE; //don't trigger on changes to the widgets
	//reset palette
	resetPalette();
	if (!clear) {
		QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM Employees WHERE EmployeeID = '" + PayrollMainWindow::instance()->currentEmployeeID
							     + "'");
		qu.first();
		ui->txtFirstName->setText(qu.record().value("FirstName").toString());
		ui->txtMiddleName->setText(qu.record().value("MiddleName").toString());
		ui->txtLastName->setText(qu.record().value("LastName").toString());

		editMode = SINGLE_EMPLOYEE_DISPLAY;

		modified = false;
	} else {
		/* clear the fields */
		Publics::clearTextBoxes(this);
	}
}

void EmployeeEditor::hideMessage()
{

}

void EmployeeEditor::closeMessage()
{
	ui->basicDetailsMessage->animatedHide();
	ui->basicDetailsMessage->setCloseButtonVisible(false);
}

void EmployeeEditor::displayMessage(QString str)
{
	ui->basicDetailsMessage->setCloseButtonVisible(false);
	ui->basicDetailsMessage->animatedShow();
	ui->basicDetailsMessage->setText(str);
}

void EmployeeEditor::enableEdition(EmployeeEditor::EditMode newEditMode)
{
//	if (((newEditMode == SINGLE_EMPLOYEE_DISPLAY || newEditMode == NONE) && PayrollMainWindow::instance()->currentEmployeeID.length() < 0
//		) || editMode != NONE)
//		return;

	if (newEditMode == IGNORE)
		return;

	if (newEditMode == SINGLE_EMPLOYEE_DISPLAY)
	{
		PayrollMainWindow::instance()->editCurrentEmployee();;
		return;
	}

	displayMessage("This employee is being edited.");
	editMode = newEditMode != NONE ? newEditMode : SINGLE_EMPLOYEE_DISPLAY;
}

void EmployeeEditor::resetPalette()
{
	QPalette p;
	ui->txtFirstName->setPalette(p);
	ui->txtMiddleName->setPalette(p);
	ui->txtLastName->setPalette(p);
}

void EmployeeEditor::clearEmployee()
{
	Publics::clearTextBoxes(this);
}

void EmployeeEditor::markChangedWidget(QWidget *w)
{
	QPalette p;
	qreal h, s, l, a;
	enableEdition();
	qApp->palette().color(QPalette::Text).getHslF(&h, &s, &l, &a);
	p.setBrush(QPalette::Base, (l <= 0.3) ? QColor(Qt::yellow).lighter() : (l <= 0.6) ? QColor(Qt::yellow).light() : /* else */ QColor(Qt::yellow).darker(300));
	w->setPalette(p);

	if (!modified) {
		modified = true;
		emit editStatus(true);
		enableEdition();
	}
}

void EmployeeEditor::addMessageAction(QAction *action)
{
	ui->basicDetailsMessage->addAction(action);
}


void EmployeeEditor::on_txtFirstName_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	if (editMode == IGNORE)
		return;

	markChangedWidget(ui->txtFirstName);
}

void EmployeeEditor::on_txtMiddleName_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	if (editMode == IGNORE)
		return;

	markChangedWidget(ui->txtMiddleName);
}

void EmployeeEditor::on_txtLastName_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	if (editMode == IGNORE)
		return;

	markChangedWidget(ui->txtLastName);
}
