#include "ui_Form.h"

class MyForm: public QWidget
{
	Q_OBJECT

public:
	MyForm(QWidget*Parent = 0);
private:
	Ui::MyForm ui;
};