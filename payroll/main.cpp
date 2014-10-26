#include "payrollmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationName("payroll");
	a.setApplicationDisplayName("Smart Payroll");
	a.setApplicationVersion("0.1");
	a.setOrganizationName("Smart Software");
	a.setOrganizationDomain("smartsofwtare.co.ke");

	PayrollMainWindow w;
	w.show();

	return a.exec();
}
