#ifndef PAYROLLMAINWINDOW_H
#define PAYROLLMAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

class QActionGroup;
class EmployeeCentre;
class PayTypesDialog;
class CompanyInformationDialog;

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
	QSqlDatabase db;
	QString currentFilePath;
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QString curFile;
	int currentMonth;
	QString monthName;
	QString yearName;
	void updateRecentFileActions();
	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];
	QActionGroup *actionsToDisable;
	void initializeCompanyFile();
	void showQueryError(QSqlQuery qu, QString title = "Error", QString textBefore = "Ther following error has occured:", QString textAfter = "Please contact your administrator.");
	//Dialogs
	EmployeeCentre *empCentre;
	PayTypesDialog *payTypes;
	CompanyInformationDialog *companyInfo;
private slots:
	void startNewCompany();
	void openFile();
	void closeFile();
	void openRecentFile();
	void currentMonthChanged();
	void uiMonthChange();
	void on_cmdMonthNext_clicked();
	void on_cmdMonthBack_clicked();
	void on_actionPay_Types_triggered();
	void on_actionEmployee_List_triggered();
	void on_actionCompany_Info_triggered();
	void companyInformationChanged();
};

#endif // PAYROLLMAINWINDOW_H
