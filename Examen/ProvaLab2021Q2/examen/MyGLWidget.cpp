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
  camPlanta = true;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::DesactivarCamera() {
  makeCurrent();
  camPlanta = false;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::SetGir(int n) {
  makeCurrent();
  angle = float(n*M_PI/16.f);
  update();
}

void MyGLWidget::initializeGL() {
  ExamGLWidget::initializeGL();
  f = true;
  angle = 0;
  emit Desactivar();
  emit ActualitzarGir(0);
}

void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersTerra();
  creaBuffersCub();
  creaBuffersEsfera();

  // Paràmetres de l'escena - arbitraris
  centreEsc = glm::vec3 (15, 0, 10);
  radiEsc = glm::distance(centreEsc, glm::vec3(0));

  enviaPosFocus();
}

void MyGLWidget::enviaPosFocus()
{
  if (f) posFoc = glm::vec3(0,0,0);
  else {
    glm::vec4 posF = View * glm::vec4(glm::vec3(15,10,10), 1.0);
    posFoc.x = posF.x;
    posFoc.y = posF.y;
    posFoc.z = posF.z;
  }
  glUniform3fv(posfocusLoc, 1, &posFoc[0]);
}

void MyGLWidget::iniCamera ()
{
  angleX = -0.5;
  angleY = 0.5;
  camPlanta = false;
  ra = float(width())/height();
  fov = float(M_PI/3.0);
  zn = 15;
  zf = 65;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pintem l'arbre
  pintaArbre (glm::vec3(10,0,10));
  pintaArbre (glm::vec3(20,0,5));
  pintaArbre (glm::vec3(25,0,15));

  glBindVertexArray(0);
}

void MyGLWidget::modelTransformPatricio ()
{  
  TG = glm::mat4(1.f);

  TG = glm::translate(TG, glm::vec3(10, 0, 10));
  TG = glm::rotate(TG, -angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(-10, 0, -10));

  TG = glm::translate(TG, glm::vec3(15,0,10));
  TG = glm::scale(TG, glm::vec3 (2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);}

void MyGLWidget::viewTransform ()
{
  if (!camPlanta) {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
    View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
    View = lookAt(glm::vec3(15,20,10), glm::vec3(15,0,10), glm::vec3(0,0,-1));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta) {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(fov, ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::perspective(float(90.f*M_PI/180), 1.0f, 1.0f, 25.0f);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      angle += M_PI/16.f;
      emit ActualitzarGir(int(angle*16.f/M_PI)%32);
    break;
	}
  case Qt::Key_F: {
      f = !f;
      enviaPosFocus();
    break;
	}
  case Qt::Key_C: {
      if (camPlanta) emit Desactivar();
      else emit Activar();
      camPlanta = !camPlanta;
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
  if ((DoingInteractive == ROTATE) && !camPlanta)
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


