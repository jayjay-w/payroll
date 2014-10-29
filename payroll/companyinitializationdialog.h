#ifndef COMPANYINITIALIZATIONDIALOG_H
#define COMPANYINITIALIZATIONDIALOG_H

#include <QDialog>
#include <QThread>
#include <QtSql>

namespace Ui {
class CompanyInitializationDialog;
}

class CompanyInitializationDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CompanyInitializationDialog(QWidget *parent = 0);
	~CompanyInitializationDialog();
private slots:
	void threadFinished();
private:
	Ui::CompanyInitializationDialog *ui;
};

class DatabaseInitThread : public QThread {
	Q_OBJECT
public:
	explicit DatabaseInitThread(QObject *parent = 0);
	void run();
private:
	void executeInitSql(QString sql);
signals:
	void progress(int);
	void status(QString);
};

#endif // COMPANYINITIALIZATIONDIALOG_H
