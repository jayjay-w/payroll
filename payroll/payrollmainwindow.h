#ifndef PAYROLLMAINWINDOW_H
#define PAYROLLMAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

class QActionGroup;

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
private slots:
    void startNewCompany();
    void openFile();
    void closeFile();
    void openRecentFile();
    void currentMonthChanged();
    void uiMonthChange();
    void on_cmdMonthNext_clicked();
    void on_cmdMonthBack_clicked();
};

#endif // PAYROLLMAINWINDOW_H
