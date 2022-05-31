// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}


void MyGLWidget::ActivarCamera() {
  makeCurrent();
  camera2 = true;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::DesactivarCamera() {
  makeCurrent();
  camera2 = false;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::SetAltura(int n) {
  makeCurrent();
  altura = n;
  update();
}

void MyGLWidget::initializeGL ()
{
  angleX = -0.5;
  angleY = 0.5;
  

  ExamGLWidget::initializeGL();
  emit Desactivar();

  girAvio = 0;
  altura = 0;

  emit ActualitzarAltura(altura);

  //viewTransform();
  f = true;
}

void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersAvio();
  creaBuffersHangar();
  creaBuffersTerra();

  centreEsc = glm::vec3(15, 1.25, 12);
  radiEsc = glm::distance(glm::vec3(30, 2.5, 24), glm::vec3(0))/2.f;
}

void MyGLWidget::iniMaterialTerra()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.2,0.1,0.2);
  diff = glm::vec3(0,1,1);
  spec = glm::vec3(0,0,0);
  shin = 500;
}

void MyGLWidget::enviaPosFocus()
{
  if (f) posFoc = glm::vec3(0,0,0);
  else {
    modelTransformAvio();
    glm::vec3 pos = float(0.5) * (capsaAvioMax + capsaAvioMin);
    pos.z = capsaAvioMin.z;
    glm::vec4 posF = View * TGAvio * glm::vec4(pos, 1.0);
    posFoc.x = posF.x;
    posFoc.y = posF.y;
    posFoc.z = posF.z;
  }
  glUniform3fv(posfocusLoc, 1, &posFoc[0]);
}

void MyGLWidget::iniCamera ()
{
  ExamGLWidget::iniCamera();
}

void MyGLWidget::paintGL ()
{
  ExamGLWidget::paintGL();
}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, glm::vec3(15, 0, 12));
  TG = glm::rotate(TG, float(-90*M_PI/180), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2.*escalaPat, 2.*escalaPat, 2.*escalaPat));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformAvio ()
{
  TGAvio = glm::mat4(1.f);
  TGAvio = glm::translate(TGAvio, glm::vec3(15, 0, 12));
  TGAvio = glm::rotate(TGAvio, -girAvio, glm::vec3(0,1,0));
  TGAvio = glm::translate(TGAvio, glm::vec3(-15, 0, -12));

  TGAvio = glm::translate(TGAvio, glm::vec3 (25, altura, 12));
  TGAvio = glm::scale(TGAvio, glm::vec3 (1.5*escalaAvio, 1.5*escalaAvio, 1.5*escalaAvio));
  TGAvio = glm::translate(TGAvio, -centreBaseAvio);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGAvio[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camera2) {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
    View = lookAt(glm::vec3(15,3,12), glm::vec3(7,2.5,12), glm::vec3(0,1,0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camera2)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(float(90.f*M_PI/180), 1.0f, 1.0f, 15.0f);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      if (altura < 5) {
        ++altura;
        emit ActualitzarAltura(altura);
      }
    break;
	}
  case Qt::Key_Down: {
      if (altura > 0)  {
        --altura;
        emit ActualitzarAltura(altura);
      }
    break;
	}
  case Qt::Key_Right: {
      girAvio += M_PI/16.f;
    break;
	}
  case Qt::Key_F: {
      enviaPosFocus();
      f = !f;
    break;
	}
  case Qt::Key_C: {
      if (camera2) emit Desactivar();
      else emit Activar();
      camera2 = !camera2;
      viewTransform();
      projectTransform();
    break;
	}
  case Qt::Key_R: {
      initializeGL();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camera2)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (yClick - e->y()) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}


