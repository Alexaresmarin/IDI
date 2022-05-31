#include "NouGLWidget.h"

#include <iostream>

NouGLWidget::NouGLWidget(QWidget *parent) : MyGLWidget(parent) {
  grabKeyboard();
}

NouGLWidget::~NouGLWidget ()
{
}

void NouGLWidget::paintGL() {
  MyGLWidget::paintGL();
}

void NouGLWidget::iniCamera ()
{
  MyGLWidget::iniCamera();
}

void NouGLWidget::iniEscena ()
{
  MyGLWidget::iniEscena();
}

void NouGLWidget::modelTransformPatricio ()
{
  MyGLWidget::modelTransformPatricio();
}

void NouGLWidget::viewTransform() {
  MyGLWidget::viewTransform();
}

void NouGLWidget::projectTransform() {
  MyGLWidget::projectTransform();
}

void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: {

      break;
    }
    case Qt::Key_W: {

      break;
    }
    case Qt::Key_A: {

      break;
    }
    case Qt::Key_D: {
    
      break;
    }
    case Qt::Key_C: {
    
      break;
    }
    default: {
      MyGLWidget::keyPressEvent(event);
      break;
    }
  }
  update();
}

  
