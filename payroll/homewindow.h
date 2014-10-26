#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QDialog>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QDialog
{
	Q_OBJECT

public:
	explicit HomeWindow(QWidget *parent = 0);
	~HomeWindow();

private:
	Ui::HomeWindow *ui;
};

#endif // HOMEWINDOW_H
