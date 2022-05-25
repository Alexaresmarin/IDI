#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) 
	:QLabel(parent) {
		// Inicialitzacio datributs si cal
}

void MyLabel::Text(QString s) {
	text = s;
}

// implementacio slots
void MyLabel::truncarLabel(int n) 
{
	QString str = text;
	str.truncate(n);
	setText(str); // heredat de QLabel
}
