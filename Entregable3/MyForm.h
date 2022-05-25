// MyForm.h – Ui::MyForm com a membre
#include "ui_MyForm.h"
class MyForm : public QWidget
{
	Q_OBJECT
 public:
	MyForm(QWidget *parent = 0);
 private:
	Ui::MyForm ui;
};
