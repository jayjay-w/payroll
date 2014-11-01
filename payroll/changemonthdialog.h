#ifndef CHANGEMONTHDIALOG_H
#define CHANGEMONTHDIALOG_H

#include <QDialog>

class QTreeWidgetItem;

namespace Ui {
class ChangeMonthDialog;
}

class ChangeMonthDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ChangeMonthDialog(QWidget *parent = 0);
	~ChangeMonthDialog();
	void reloadMonths();

signals:
	void monthChanged(QString monthID);
private slots:
	void on_trvMonths_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_cmdSetMonth_clicked();

private:
	Ui::ChangeMonthDialog *ui;
};

#endif // CHANGEMONTHDIALOG_H
