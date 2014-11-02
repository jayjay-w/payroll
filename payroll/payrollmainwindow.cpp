#include "payrollmainwindow.h"
#include "ui_payrollmainwindow.h"
#include "publics.h"
#include "companyinitializationdialog.h"
#include <QActionGroup>
#include "paytypesdialog.h"
#include "employeecentre.h"
#include "companyinformationdialog.h"
#include "changemonthdialog.h"
#include "aboutdialog.h"
#include "startnewemployeedialog.h"

PayrollMainWindow *PayrollMainWindow::m_instance = NULL;

PayrollMainWindow::PayrollMainWindow() : QMainWindow(),
	empCentre(0),
	monthChanger(0),
	actionsToDisable(0),
	payTypes(0),
	companyInfo(0),
	aboutPayroll(new AboutDialog(this)),
	ui(new Ui::PayrollMainWindow)
{
	Q_ASSERT_X(m_instance == NULL, "MainWindow", "MainWindow recreated!");
	m_instance = this;
	ui->setupUi(this);
	//ActionGroup
	if (!actionsToDisable)
		actionsToDisable = new QActionGroup(this);

	actionsToDisable->addAction(ui->actionAttendance);
	actionsToDisable->addAction(ui->actionAttendance_2);
	//actionsToDisable->addAction(ui->actionBackup_Restore);
	actionsToDisable->addAction(ui->actionClose_Company_Logoff);
	actionsToDisable->addAction(ui->actionClose_Month);
	actionsToDisable->addAction(ui->actionCompany_Info);
	actionsToDisable->addAction(ui->actionDepartments);
	actionsToDisable->addAction(ui->actionEmployee_List);
	actionsToDisable->addAction(ui->actionEnter_Leave);
	actionsToDisable->addAction(ui->actionEnter_OT);
	actionsToDisable->addAction(ui->actionJob_Groups);
	actionsToDisable->addAction(ui->actionJob_Titles);
	actionsToDisable->addAction(ui->actionLeave);
	actionsToDisable->addAction(ui->actionLoans);
	actionsToDisable->addAction(ui->actionLoan_Schedule);
	actionsToDisable->addAction(ui->actionPay);
	actionsToDisable->addAction(ui->actionTerminate);
	actionsToDisable->addAction(ui->actionPayroll);
	actionsToDisable->addAction(ui->actionPayroll_Master);
	actionsToDisable->addAction(ui->actionPayslips);
	actionsToDisable->addAction(ui->actionPayslips_2);
	actionsToDisable->addAction(ui->actionPay_Types);
	actionsToDisable->addAction(ui->actionRecruit);
	actionsToDisable->addAction(ui->actionStatutory_Rates);
	actionsToDisable->addAction(ui->actionUser_Manager);
	actionsToDisable->addAction(ui->actionChange_Month);

	actionsToDisable->setDisabled(true);
	//Recent file actions
	ui->menuRecent_Files->clear();
	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect (recentFileActs[i], SIGNAL(triggered()), SLOT(openRecentFile()));
		ui->menuRecent_Files->addAction(recentFileActs[i]);
	}
	updateRecentFileActions();
	//
	connect (ui->action_New, SIGNAL(triggered()), SLOT(startNewCompany()));
	connect (ui->action_Open, SIGNAL(triggered()), SLOT(openFile()));
	connect (ui->actionClose_Company_Logoff, SIGNAL(triggered()), SLOT(closeFile()));
	//
	closeFile();
	this->setWindowState(Qt::WindowMaximized);
	this->restoreState(Publics::getSetting("MainWindowState", this->saveState()).toByteArray());
	ui->employeeDetailsWidget->resize(100, ui->employeeDetailsWidget->height());
	ui->trvEmployees->resize(ui->trvEmployees->width(), 3000);
}

PayrollMainWindow::~PayrollMainWindow()
{
	Publics::saveSetting("MainWindowSize", this->geometry());
	Publics::saveSetting("MainWindowState", this->saveState());
	delete ui;
}

PayrollMainWindow *PayrollMainWindow::instance()
{
	return m_instance;
}

void PayrollMainWindow::loadFile(const QString &fileName)
{
	qDebug() << fileName;

	if (fileName.isNull()) {
		openFile();
		return;
	}

	closeFile();
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(fileName);
	if (!db.open()) {
		QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1").arg(db.lastError().text()));
		return;
	}

	QSqlQuery q("SELECT * FROM company WHERE appName='smpayroll'", db);
	if (!q.exec()) {
		//Database Open Error
		QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1 is probably not a valid payroll file").arg(fileName));
		return;
	}
	//Database open
	initializeCompanyFile();
	//Check for currentmonth
	QString curMonthID = Publics::getDbValue("SELECT * FROM Company", "CurrentMonth").toString();
	qDebug() << curMonthID;
	if (curMonthID.length() < 1) {
		//No current month specified, so use the current calendar month
		QString cMonthID = Publics::getDbValue("SELECT * FROM PayrollMonths WHERE Year = '"
						       + QString::number(QDate::currentDate().year())
						       + "' AND Month = '" + QDate::currentDate().toString("MMMM") + "'", "PayrollMonthID").toString();
		db.exec("UPDATE Company SET CurrentMonth = '" + cMonthID + "'");
		curMonthID  = cMonthID;
	}
	currentMonth = curMonthID.toInt();
	uiMonthChange(QString::number(currentMonth));
	curFile = fileName;
	ui->statusBar->showMessage(QString("Opened file: %1").arg(fileName));
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	QStringList files = sett.value("recentFiles").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);

	while (files.size() > MaxRecentFiles)
		files.removeLast();

	sett.setValue("recentFiles", files);
	updateRecentFileActions();
	actionsToDisable->setEnabled(true);
	ui->action_Save->setEnabled(true);
	ui->actionSave_As->setEnabled(true);
}

bool PayrollMainWindow::saveFile(const QString &fileName)
{
	Q_UNUSED(fileName);
	return false;
}

void PayrollMainWindow::setCurrentFile(const QString &fileName)
{
	Q_UNUSED(fileName);
}

QString PayrollMainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void PayrollMainWindow::updateRecentFileActions()
{
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	QStringList files = sett.value("recentFiles").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString fNum = QString::number(i + 1);

		if (i < 9)
			fNum.prepend(" ");

		QString text = tr("%1 %2").arg(fNum).arg(strippedName(files[i]));
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setStatusTip(files[i]);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

}

void PayrollMainWindow::initializeCompanyFile()
{
	CompanyInitializationDialog *init = new CompanyInitializationDialog(this);
	init->exec();
}

void PayrollMainWindow::showQueryError(QSqlQuery qu, QString title, QString textBefore, QString textAfter)
{
	QMessageBox::critical(this, title, QString("%1<br/><b>%2</b><br/>%3").arg(textBefore, qu.lastError().text(), textAfter));
}

void PayrollMainWindow::startNewCompany()
{
	NewCompanyDialog *newC = new NewCompanyDialog(this);
	if (newC->exec() == QDialog::Accepted) {
		//loadFile(newC->companyFileLocation);
		this->loadFile(newC->companyFileLocation);
	}
}

void PayrollMainWindow::openFile()
{
	QString fileName = Publics::getOpenFile(this);
	if (fileName.length() > 0) {
		loadFile(fileName);
	}
}

void PayrollMainWindow::closeFile()
{
	ui->lblCompanyAndCurrentMonth->setText("No company open");
	actionsToDisable->setEnabled(false);
	ui->action_Save->setEnabled(false);
	ui->actionSave_As->setEnabled(false);
	curFile = "";
	if (db.isOpen())
		db.close();
}

void PayrollMainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}

void PayrollMainWindow::currentMonthChanged()
{
	QString companyName = Publics::getDbValue("SELECT * FROM Company", "CompanyName").toString();
	QString s_currentMonth = currentYearName + " " + currentMonthName;
	ui->lblCompanyAndCurrentMonth->setText(companyName + "-" + s_currentMonth);
}

void PayrollMainWindow::uiMonthChange(QString newMonthID)
{
	if (newMonthID.length() < 1)
		newMonthID = "1";

	currentYearName = Publics::getDbValue("SELECT * FROM PayrollMonths WHERE PayrollMonthID = '"
						       + newMonthID + "'", "Year").toString();

	currentMonthName = Publics::getDbValue("SELECT * FROM PayrollMonths WHERE PayrollMonthID = '"
						       + newMonthID + "'", "Month").toString();

	if (newMonthID.length() < 1) {
		QMessageBox::critical(this, "Error", "Month not found.");
		return;
	}

	db.exec("UPDATE Company SET CurrentMonth = '" + newMonthID + "'");
	currentMonth = newMonthID.toInt();
	currentMonthChanged();
}

void PayrollMainWindow::on_actionPay_Types_triggered()
{
	if (!payTypes)
		payTypes = new PayTypesDialog(this);

	payTypes->exec();
}

void PayrollMainWindow::on_actionEmployee_List_triggered()
{
	if (!empCentre)
		empCentre = new EmployeeCentre(this);

	empCentre->reloadEmployeeList();
	empCentre->exec();
}

void PayrollMainWindow::on_actionCompany_Info_triggered()
{
	if (!companyInfo) {
		companyInfo = new CompanyInformationDialog(this);
		connect (companyInfo, SIGNAL(dataChanged()), SLOT(companyInformationChanged()));
	}

	companyInfo->exec();
}

void PayrollMainWindow::companyInformationChanged()
{
	uiMonthChange(QString::number(currentMonth));
}

void PayrollMainWindow::on_actionChange_Month_triggered()
{
	if (!monthChanger) {
		monthChanger = new ChangeMonthDialog(this);
		connect (monthChanger, SIGNAL(monthChanged(QString)), SLOT(uiMonthChange(QString)));
	}

	monthChanger->exec();
}

void PayrollMainWindow::on_actionAbout_Payroll_triggered()
{
	if (!aboutPayroll)
		aboutPayroll = new AboutDialog(this);

	aboutPayroll->exec();
}

void PayrollMainWindow::on_actionRecruit_triggered()
{
	StartNewEmployeeDialog *startN = new StartNewEmployeeDialog(this);
	if (startN->exec() == QDialog::Accepted)
	{
		currentEmployee = startN->empID;
		on_actionEmployee_List_triggered();
	}
}
