#include "MyLineEdit.h"

// constructor
MyLineEdit::MyLineEdit(QWidget *parent)
	: QLineEdit(parent) {
		// Inicialitzacio datributs si cal
		connect(this, SIGNAL(enviaText()), this, SLOT(tractaReturn()));
}

// implementacio slots
void MyLineEdit::tractaReturn() {
	// Implementacio de tractaReturn
	emit enviaText(text());
}