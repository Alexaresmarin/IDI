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

void MyGLWidget::initializeGL() {
  LL4GLWidget::initializeGL();
  LL4GLWidget::calculaCapsaModel();
  
  carregaShaders();

  angle = 0;
  escena = camera = pat = true;
  modelTransformFocus();

  glm::vec3 colFocusCamera = glm::vec3(0.9, 0.0, 0.9);
  glUniform3fv(colFocusCam, 1, &colFocusCamera[0]);

  glm::vec3 posFocusPatricio = glm::vec3(5, 3, 2);
  glm::vec3 colFocusPatricio = glm::vec3(0.9, 0.9, 0.2);
  glUniform3fv(posFocusPat, 1, &posFocusPatricio[0]);
  glUniform3fv(colFocusPat, 1, &colFocusPatricio[0]);

  glm::vec3 posFocusEscena = glm::vec3(10.0, 3.0, 5.0);
  glm::vec3 colFocusEscena = glm::vec3(0.9, 0.9, 0.9);
  glUniform3fv(posFocusEsc, 1, &posFocusEscena[0]);
  glUniform3fv(colFocusEsc, 1, &colFocusEscena[0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_C: {
      glm::vec3 colFocusCamera;
      if (camera) colFocusCamera = glm::vec3(0.0,0.0,0.0);
      else colFocusCamera = glm::vec3(0.9,0.0,0.9);
      glUniform3fv(colFocusCam, 1, &colFocusCamera[0]);
      camera = !camera;
    break;
	}
  case Qt::Key_E: {
      glm::vec3 colFocusEscena;
      if (escena) colFocusEscena = glm::vec3(0.0,0.0,0.0);
      else colFocusEscena = glm::vec3(0.9,0.0,0.9);
      glUniform3fv(colFocusEsc, 1, &colFocusEscena[0]);
      escena = !escena;
    break;
	}
  case Qt::Key_P: {
      glm::vec3 colFocusPatricio;
      if (pat) colFocusPatricio = glm::vec3(0.0,0.0,0.0);
      else colFocusPatricio = glm::vec3(0.9,0.0,0.9);
      glUniform3fv(colFocusPat, 1, &colFocusPatricio[0]);
      pat = !pat;
    break;
	}
  case Qt::Key_Right: {
      angle -= float(30.0f*M_PI/180.0f);
      modelTransformFocus();
    break;
	}
  case Qt::Key_Left: {
      angle += float(30.0f*M_PI/180.0f);
      modelTransformFocus();
    break;
	}
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5.0,0.0,5.0));
  TG = glm::rotate(TG, angle, glm::vec3(0.0,1.0,0.0));
  TG = glm::translate(TG, glm::vec3(-5.0,0.0,-5.0)); 

  // El moc a la posició (0,0,-3) per aconseguir un radi de 3

  TG = glm::translate(TG, glm::vec3(5,0,2));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFocus() {
  TGaux = glm::translate(glm::mat4(1.f), glm::vec3(5.0,3.0,5.0));
  TGaux = glm::rotate(TGaux, angle, glm::vec3(0.0,1.0,0.0)); // El moc a la posició (0,0,-3) per aconseguir un radi de 3
  TGaux = glm::translate(TGaux, glm::vec3(-5.0,-3.0,-5.0));

  glUniformMatrix4fv (transFocusLoc, 1, GL_FALSE, &TGaux[0][0]);
}

void MyGLWidget::carregaShaders() {
  colFocusCam = glGetUniformLocation (program->programId(), "colFocusCam");
  posFocusPat = glGetUniformLocation (program->programId(), "posFocusPat");
  colFocusPat = glGetUniformLocation (program->programId(), "colFocusPat");
  posFocusEsc = glGetUniformLocation (program->programId(), "posFocusEsc");
  colFocusEsc = glGetUniformLocation (program->programId(), "colFocusEsc");
  transFocusLoc = glGetUniformLocation (program->programId(), "TGaux");
}