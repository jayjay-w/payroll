#include "payrollmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PayrollMainWindow w;
	w.show();

	return a.exec();
}
