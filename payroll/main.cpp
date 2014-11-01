#include "payrollmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationName("payroll");
#if QT_VERSION > 0x50000
	a.setApplicationDisplayName("Smart Payroll");
#endif
	a.setApplicationVersion("0.1");
	a.setOrganizationName("Smart Software");
	a.setOrganizationDomain("smartsofwtare.co.ke");

	PayrollMainWindow w;
	w.show();

	return a.exec();
}
