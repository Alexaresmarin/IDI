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

void MyGLWidget::activarOrtho() {
  makeCurrent();  
  camPlanta = true;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::activarPers() {
  makeCurrent();  
  camPlanta = false;
  projectTransform();
  viewTransform();
  update();
}

void MyGLWidget::AsignarPatCub(int x) {
  makeCurrent();
  esrota = x-1;
  update();
}

void MyGLWidget::initializeGL ()
{
  ExamGLWidget::initializeGL();
  pintaPat = false;
  esrota = 0;

  emit ActivarPers();
  emit AsignarPat(1);

  rotacio[0] = 0;
  rotacio[1] = 1;
  rotacio[2] = 2;
  angles[0] = 0;
  angles[1] = float(2.*M_PI/3);
  angles[2] = float(4.*M_PI/3);

  blanc = true;
  colFocus = glm::vec3 (1, 1, 1);
  glUniform3fv (colfocusLoc, 1, &colFocus[0]);
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
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
  if (pintaPat) {
    // Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  else {
    // Pintem el cub
    glBindVertexArray(VAO_Cub);
    modelTransformCub (2.0/0.5, angles[rotacio[0]]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    modelTransformCub (2.5/0.5, angles[rotacio[1]]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    modelTransformCub (3/0.5, angles[rotacio[2]]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  TG = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3(5, 0, 0));

  TG = glm::scale(TG, glm::vec3(escala));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(glm::mat4(1.f), angles[rotacio[esrota]], glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3 (5, 0, 0));
  TG = glm::scale(TG, glm::vec3 (2.*escala));
  TG = glm::rotate(TG, float(-90.*M_PI/180), glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::lookAt(glm::vec3(0,2*radiEsc,0), glm::vec3(0,0,0), glm::vec3(1,0,0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    if (ra >= 1) Proj = glm::ortho(-ra*radiEsc, ra*radiEsc, -radiEsc, radiEsc, zn, zf);
    else Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc/ra, radiEsc/ra, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      pintaPat = !pintaPat;
    break;
	}
  case Qt::Key_1: {
      esrota = rotacio[0];
      emit AsignarPat(1);
    break;
	}
  case Qt::Key_2: {
      esrota = rotacio[1];
      emit AsignarPat(2);
    break;
	}
  case Qt::Key_3: {
      esrota = rotacio[2];
      emit AsignarPat(3);
    break;
	}
  case Qt::Key_F: {
      if (blanc) colFocus = glm::vec3 (1, 1, 0);
      else colFocus = glm::vec3 (1, 1, 1);
      blanc = !blanc;
      glUniform3fv (colfocusLoc, 1, &colFocus[0]);
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      projectTransform();
      viewTransform();
      if (camPlanta) emit ActivarOrtho();
      else emit ActivarPers();
    break;
	}
  case Qt::Key_Right: {
      if (!pintaPat) {
        int aux = rotacio[0];
        rotacio[0] = rotacio[1];
        rotacio[1] = rotacio[2];
        rotacio[2] = aux;
      }
    break;
	}
  case Qt::Key_Left: {
      if (!pintaPat) {
        int aux = rotacio[2];
        rotacio[2] = rotacio[1];
        rotacio[1] = rotacio[0];
        rotacio[0] = aux;
      }
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

