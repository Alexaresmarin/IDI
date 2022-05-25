#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) 
	:QLabel(parent) {
		// Inicialitzacio datributs si cal
}

void MyLabel::R(int n) {
	r = n;
	std::string s = "background-color: rgb(";
	s += std::to_string(r);
	s += ",";
	s += std::to_string(g);
	s += ",";
	s += std::to_string(b);
	s += ");";
	QString str = QString::fromUtf8(s.c_str());
	setStyleSheet(str);
	emit EnviarRGB(float(r/255.0f), float(g/255.0f), float(b/255.0f));
}

void MyLabel::G(int n) {
	g = n;
	std::string s = "background-color: rgb(";
	s += std::to_string(r);
	s += ",";
	s += std::to_string(g);
	s += ",";
	s += std::to_string(b);
	s += ");";
	QString str = QString::fromUtf8(s.c_str());
	setStyleSheet(str);
	emit EnviarRGB(float(r/255.0f), float(g/255.0f), float(b/255.0f));
}

void MyLabel::B(int n) {
	b = n;
	std::string s = "background-color: rgb(";
	s += std::to_string(r);
	s += ",";
	s += std::to_string(g);
	s += ",";
	s += std::to_string(b);
	s += ");";
	QString str = QString::fromUtf8(s.c_str());
	setStyleSheet(str);
	emit EnviarRGB(float(r/255.0f), float(g/255.0f), float(b/255.0f));
}