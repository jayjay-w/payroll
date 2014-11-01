#include "changemonthdialog.h"
#include "ui_changemonthdialog.h"
#include <QtSql>
#include "payrollmainwindow.h"

ChangeMonthDialog::ChangeMonthDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ChangeMonthDialog)
{
	ui->setupUi(this);
	reloadMonths();
}

ChangeMonthDialog::~ChangeMonthDialog()
{
	delete ui;
}

void ChangeMonthDialog::reloadMonths()
{
	ui->trvMonths->invisibleRootItem()->takeChildren();

	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM PayrollMonths");

	if (!qu.lastError().isValid()) {
		while (qu.next()) {
			QString monthID = qu.record().value("PayrollMonthID").toString();
			QString month = qu.record().value("Month").toString();
			QString year = qu.record().value("Year").toString();

			int closed = qu.record().value("Closed").toInt();
			QString status = "Open";
			if (closed == 1) {
				status = "Closed";
			}

			QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvMonths);
			it->setText(10, monthID);
			it->setText(0, year + " " + month);
			it->setText(1, status);

			if (monthID == QString::number(PayrollMainWindow::instance()->currentMonth)) {
				//Current month
				QFont fnt = it->font(0);
				fnt.setBold(true);
				it->setFont(0, fnt);
				it->setFont(1, fnt);
				it->setText(1, it->text(1) + "-[Current Month]");
				it->setBackgroundColor(0, QColor(Qt::red));
				it->setBackgroundColor(1, QColor(Qt::red));

				ui->trvMonths->scrollToItem(it);
			}
		}
	}

	ui->trvMonths->resizeColumnToContents(0);
	ui->trvMonths->resizeColumnToContents(1);
}

void ChangeMonthDialog::on_trvMonths_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	emit monthChanged(item->text(10));
	reloadMonths();
}

void ChangeMonthDialog::on_cmdSetMonth_clicked()
{
	if (ui->trvMonths->selectedItems() .count() < 1) {
		return;
	}
	QTreeWidgetItem *selectedItem = ui->trvMonths->selectedItems().at(0);
	emit monthChanged(selectedItem->text(10));
	reloadMonths();
	this->accept();
}
