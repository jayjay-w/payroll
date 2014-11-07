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

	setTXTEvtFilters(this);
	connectTXT(this);

	connect (ui->cmdAddNew, SIGNAL(clicked()), SLOT(startAddEmployee()));
	connect (ui->cmdDelete, SIGNAL(clicked()), SLOT(deleteEmployee()));

	connect (ui->cboDepartment, SIGNAL(currentIndexChanged(int)), SLOT(cboChanged()));
	connect (ui->cboJobGroup, SIGNAL(currentIndexChanged(int)), SLOT(cboChanged()));
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
	QString query = "SELECT * FROM Company";
	QString dept_id = Publics::getDbValue("SELECT * FROM Departments WHERE Department = '" + ui->cboDepartment->currentText() + "'", "DepartmentID").toString();
	QString j_grp_id = Publics::getDbValue("SELECT * FROM JobGroups WHERE JobGroup = '" + ui->cboJobGroup->currentText() + "'", "JobGroupID").toString();;
	if (editMode == SINGLE_EMPLOYEE_EDIT || editMode == SINGLE_EMPLOYEE_DISPLAY)
		query = "UPDATE Employees SET "
				"FirstName = '" + ui->txtFirstName->text()
				+ "', MiddleName = '" + ui->txtMiddleName->text()
				+ "', LastName = '" + ui->txtLastName->text()
				+ "', IDNo = '" + ui->txtIDNo->text()
				+ "', PINNo = '" + ui->txtPINNo->text()
				+ "', NHIFNo = '" + ui->txtNHIFNo->text()
				+ "', NSSFNo = '" + ui->txtNSSFNo->text()
				+ "', PhoneNo = '" + ui->txtTel->text()
				+ "', Email = '" + ui->txtEmail->text()
				+ "', Address = '" + ui->txtAddress->text()
				+ "', Postcode = '" + ui->txtPostcode->text()
				+ "', DepartmentID = '" + dept_id
				+ "', GroupID = '" + j_grp_id
				+ "', Town = '" + ui->txtTown->text()
				+ "' WHERE EmployeeID = '"
				+ PayrollMainWindow::instance()->currentEmployeeID + "'";

	if (editMode == ADD)
		query = "INSERT INTO Employees "
				"(FirstName, MiddleName, LastName, IDNo, PINNo, NSSFNo, "
				"NHIFNo, PhoneNo, Email, DepartmentID, GroupID, Address, Postcode, Town"
				") VALUES "
				"('" + ui->txtFirstName->text()
				+ "', '" + ui->txtMiddleName->text()
				+ "', '" + ui->txtLastName->text()
				+ "', '" + ui->txtIDNo->text()
				+ "', '" + ui->txtPINNo->text()
				+ "', '" + ui->txtNSSFNo->text()
				+ "', '" + ui->txtNHIFNo->text()
				+ "', '" + ui->txtTel->text()
				+ "', '" + ui->txtEmail->text()
				+ "', '" + dept_id
				+ "', '" + j_grp_id
				+ "', '" + ui->txtAddress->text()
				+ "', '" + ui->txtPostcode->text()
				+ "', '" + ui->txtTown->text()
				+ "')";

	QSqlQuery qu = QSqlDatabase::database().exec(query);

	if (qu.lastError().isValid()) {
		//In case of an error, show message and stop right here
		PayrollMainWindow::instance()->showQueryError(qu);
		return;
	}

	//No error, reset UI
	modified = false;
	emit editStatus(false);
	closeMessage();
	resetPalette();

	ui->cmdAddNew->setEnabled(true);
	ui->cmdDelete->setEnabled(true);

	if (editMode == ADD)
		PayrollMainWindow::instance()->employeeChanged(qu.lastInsertId().toString());

	enableEdition(SINGLE_EMPLOYEE_DISPLAY);
	showEmployeeDetails(false);
	emit employeeChanged();
}

void EmployeeEditor::rejectChanges()
{
	editMode = NONE;
	emit editStatus(false);
	modified = false;
	enableEdition(IGNORE);
	closeMessage();
	resetPalette();
	ui->cmdAddNew->setEnabled(true);
	ui->cmdDelete->setEnabled(true);
	//Revert to original details
	showEmployeeDetails(false);
}

void EmployeeEditor::showEmployeeDetails(bool clear)
{
	//don't execute this while adding / editing an employee
	if (editMode == ADD || editMode == SINGLE_EMPLOYEE_EDIT) {
		return;
	}


	if (!isEnabled() && !clear)
		setEnabled(true);

	if (isEnabled() && clear)
		setEnabled(false);

	editMode = IGNORE; //don't trigger on changes to the widgets
	//reset palette
	resetPalette();
	if (!clear) {
		Publics::loadQueryToCombo("SELECT * FROM Departments", "Department", ui->cboDepartment);
		Publics::loadQueryToCombo("SELECT * FROM JobGroups", "JobGroup", ui->cboJobGroup);
		QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM Employees WHERE EmployeeID = '" + PayrollMainWindow::instance()->currentEmployeeID
							     + "'");
		qu.first();
		ui->txtFirstName->setText(qu.record().value("FirstName").toString());
		ui->txtMiddleName->setText(qu.record().value("MiddleName").toString());
		ui->txtLastName->setText(qu.record().value("LastName").toString());
		ui->txtIDNo->setText(qu.record().value("IDNo").toString());
		ui->txtPINNo->setText(qu.record().value("PINNo").toString());
		ui->txtNSSFNo->setText(qu.record().value("NSSFNo").toString());
		ui->txtNHIFNo->setText(qu.record().value("NHIFNo").toString());
		ui->txtTel->setText(qu.record().value("PhoneNo").toString());
		ui->txtEmail->setText(qu.record().value("Email").toString());
		ui->txtAddress->setText(qu.record().value("Address").toString());
		ui->txtPostcode->setText(qu.record().value("Postcode").toString());
		ui->txtTown->setText(qu.record().value("Town").toString());

		QString dept_id = qu.record().value("DepartmentID").toString();
		QString grp_id = qu.record().value("GroupID").toString();

		ui->cboDepartment->setCurrentIndex(0);
		ui->cboJobGroup->setCurrentIndex(0);

		Publics::setComboBoxText(ui->cboDepartment, Publics::getDbValue("SELECT * FROM Departments WHERE DepartmentID = '" + dept_id + "'", "Department").toString());
		Publics::setComboBoxText(ui->cboJobGroup, Publics::getDbValue("SELECT * FROM JobGroups WHERE JobGroupID = '" + grp_id + "'", "JobGroup").toString());

		editMode = SINGLE_EMPLOYEE_DISPLAY;

		modified = false;
	} else {
		/* clear the fields */
		Publics::clearTextBoxes(this);
		ui->cboDepartment->setCurrentIndex(0);
		ui->cboJobGroup->setCurrentIndex(0);
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
	if (newEditMode == IGNORE)
		return;

	if (newEditMode == SINGLE_EMPLOYEE_EDIT || newEditMode == ADD) {
		ui->cmdAddNew->setEnabled(false);
		ui->cmdDelete->setEnabled(false);
	}

	if (newEditMode == SINGLE_EMPLOYEE_EDIT) {
		displayMessage("This employee is being edited.");
		editMode = newEditMode;
		return;
	}

	if (newEditMode == ADD) {
		displayMessage("Adding a new employee");
		editMode = newEditMode;
		return;
	}

	editMode = newEditMode != NONE ? newEditMode : SINGLE_EMPLOYEE_DISPLAY;
}

void EmployeeEditor::resetPalette()
{
	resetTXTPalette(this);
}

void EmployeeEditor::clearEmployee()
{
	Publics::clearTextBoxes(this);
	ui->cboDepartment->setCurrentIndex(0);
	ui->cboJobGroup->setCurrentIndex(0);
}

void EmployeeEditor::startAddEmployee()
{
	enableEdition(ADD);
	Publics::clearTextBoxes(this);
	ui->cboDepartment->setCurrentIndex(0);
	ui->cboJobGroup->setCurrentIndex(0);
}

void EmployeeEditor::deleteEmployee()
{
	if (editMode == SINGLE_EMPLOYEE_DISPLAY) {
		//In display mode of a single employee. Start deletion
		if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this employee?", QMessageBox::Yes, QMessageBox::No) ==QMessageBox::No )
			return;

		//All OK
		QSqlDatabase::database().exec("DELETE FROM Employees WHERE EmployeeID = '" + PayrollMainWindow::instance()->currentEmployeeID + "'");
		emit employeeDeleted();
		editMode = IGNORE;
		enableEdition(IGNORE);
		Publics::clearTextBoxes(this);
		ui->cboDepartment->setCurrentIndex(0);
		ui->cboJobGroup->setCurrentIndex(0);
	}
}

void EmployeeEditor::lineEditTextChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	if (editMode == IGNORE)
		return;

	if (editMode == ADD) {
		enableEdition(ADD);
		return;
	}

	QLineEdit *txt = qobject_cast<QLineEdit *>(sender());
	markChangedWidget(txt);
}

void EmployeeEditor::cboChanged()
{
	if (editMode == IGNORE)
		return;

	if (editMode == ADD) {
		enableEdition(ADD);
		return;
	}

	if (!modified) {
		modified = true;
		emit editStatus(true);
		enableEdition();
		if (editMode == SINGLE_EMPLOYEE_DISPLAY)
			enableEdition(SINGLE_EMPLOYEE_EDIT);
	}
}

void EmployeeEditor::setTXTEvtFilters(QWidget *parent)
{
	QPalette p;
	for (int i = 0; i < parent->children().count(); i++) {
		if (qobject_cast<QLineEdit *>(parent->children().at(i))) {
			QLineEdit *txt = qobject_cast<QLineEdit *>(parent->children().at(i));
			txt->installEventFilter(this);
		}
		if (parent->children().at(i)->children().count() > 0) {
			if (qobject_cast<QWidget *>(parent->children().at(i))) {
				QWidget *wid = qobject_cast<QWidget *>(parent->children().at(i));
				setTXTEvtFilters(wid);
			}
		}
	}
}


void EmployeeEditor::connectTXT(QWidget *parent)
{
	QPalette p;
	for (int i = 0; i < parent->children().count(); i++) {
		if (qobject_cast<QLineEdit *>(parent->children().at(i))) {
			QLineEdit *txt = qobject_cast<QLineEdit *>(parent->children().at(i));
			connect(txt, SIGNAL(textChanged(QString)), SLOT(lineEditTextChanged(QString)));
		}
		if (parent->children().at(i)->children().count() > 0) {
			if (qobject_cast<QWidget *>(parent->children().at(i))) {
				QWidget *wid = qobject_cast<QWidget *>(parent->children().at(i));
				connectTXT(wid);
			}
		}
	}
}

void EmployeeEditor::resetTXTPalette(QWidget *parent)
{
	QPalette p;
	for (int i = 0; i < parent->children().count(); i++) {
		if (qobject_cast<QLineEdit *>(parent->children().at(i))) {
			QLineEdit *txt = qobject_cast<QLineEdit *>(parent->children().at(i));
			txt->setPalette(p);
		}
		if (parent->children().at(i)->children().count() > 0) {
			if (qobject_cast<QWidget *>(parent->children().at(i))) {
				QWidget *wid = qobject_cast<QWidget *>(parent->children().at(i));
				resetTXTPalette(wid);
			}
		}
	}
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
		if (editMode == SINGLE_EMPLOYEE_DISPLAY)
			enableEdition(SINGLE_EMPLOYEE_EDIT);
	}
}

void EmployeeEditor::addMessageAction(QAction *action)
{
	ui->basicDetailsMessage->addAction(action);
}


