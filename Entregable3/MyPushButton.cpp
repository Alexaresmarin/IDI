#include "MyPushButton.h"

    int MyPushButton::n = 0;
    int MyPushButton::rec1 = -1;
    int MyPushButton::rec2 = -1;
    int MyPushButton::rec3 = -1;

MyPushButton::MyPushButton(QWidget *parent) 
	:QPushButton(parent) {
		// Inicialitzacio datributs si cal
}

void MyPushButton::tractar_rectangle() {
    emit pinta();
    if ((rec1 == rec2) and (rec2 == rec3)) {
        ++n;
        emit set(n);
    }
}

void MyPushButton::canviar_color_rec() {
    int x = rand()%4;
    if (x == 0) setStyleSheet("background-color: rgb(200,0,0);");
    else if (x == 1) setStyleSheet("background-color: rgb(0,200,0);");
    else if (x == 2) setStyleSheet("background-color: rgb(0,0,200);");
    else if (x == 3) setStyleSheet("background-color: rgb(200,200,0);");
    if (objectName() == "rectangle_1") rec1 = x;
    else if (objectName() == "rectangle_2") rec2 = x;
    else if (objectName() == "rectangle_3") rec3 = x;
}

void MyPushButton::canviar_color_gris() {
    setStyleSheet("background-color: rgb(124,124,124);");
}

void MyPushButton::tractar_reset() {
    n = 0;
    rec1 = rec2 = rec3 = -1;
    emit set(n);
}