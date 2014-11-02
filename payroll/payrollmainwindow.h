#ifndef PAYROLLMAINWINDOW_H
#define PAYROLLMAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

class QActionGroup;
class EmployeeCentre;
class PayTypesDialog;
class CompanyInformationDialog;
class ChangeMonthDialog;
class AboutDialog;
class EmployeeEditor;
class QTreeWidgetItem;

namespace Ui {
class PayrollMainWindow;
}

class PayrollMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit PayrollMainWindow();
	~PayrollMainWindow();
	static PayrollMainWindow *instance();
	EmployeeEditor *empEditor();
	int currentMonth;
	QString currentMonthName;
	QString currentYearName;
	QString currentEmployeeID;
	void showQueryError(QSqlQuery qu, QString title = "Error", QString textBefore = "Ther following error has occured:", QString textAfter = "Please contact your administrator.");

public slots:
	void reloadEmployees();
	void employeeChanged(QString employeeID);
	void editCurrentEmployee();
private:
	Ui::PayrollMainWindow *ui;
	QSqlDatabase db;
	QString currentFilePath;
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QString curFile;
	QString monthName;
	QString yearName;
	void updateRecentFileActions();
	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];
	QActionGroup *actionsToDisable;
	void initializeCompanyFile();static PayrollMainWindow *m_instance;
	//Dialogs
	EmployeeCentre *empCentre;
	PayTypesDialog *payTypes;
	CompanyInformationDialog *companyInfo;
	ChangeMonthDialog *monthChanger;
	AboutDialog *aboutPayroll;
private slots:
	void startNewCompany();
	void openFile();
	void closeFile();
	void openRecentFile();
	void currentMonthChanged();
	void uiMonthChange(QString newMonthID);
	void on_actionPay_Types_triggered();
	void on_actionEmployee_List_triggered();
	void on_actionCompany_Info_triggered();
	void companyInformationChanged();
	void on_actionChange_Month_triggered();
	void on_actionAbout_Payroll_triggered();
	void on_actionRecruit_triggered();
	void on_trvEmployees_itemClicked(QTreeWidgetItem *item, int column);
};

#endif // PAYROLLMAINWINDOW_H
