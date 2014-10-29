#ifndef NEWCOMPANYDIALOG_H
#define NEWCOMPANYDIALOG_H

#include <QDialog>

namespace Ui {
class NewCompanyDialog;
}

class NewCompanyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCompanyDialog(QWidget *parent = 0);
    ~NewCompanyDialog();
    QString companyFileLocation;
private slots:
    void on_cmdClose_clicked();

    void on_cmdSetFilePath_clicked();

    void on_cmdCreateCompany_clicked();

private:
    Ui::NewCompanyDialog *ui;
};

#endif // NEWCOMPANYDIALOG_H
